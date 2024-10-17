/**
 ******************************************************************************
 * @File           : STM32F103C6_I2C_Driver.c
 * @Author         : Abdelrhman Marzoq
 * @Brief          : I2C Driver
 * 			Created on: Oct 10, 2024
 ******************************************************************************
**/

#include "STM32F103C6_I2C_Driver.h"


/*
 * =======================================================================================
 * 										Generic Variables
 * =======================================================================================
*/

//#define Polling(X, EV) while(!Get_Status(X, EV))

typedef enum
{
	Busy,
	EV5,
	EV6,
	EV8_1,
	EV8,
	EV8_2,
	EV7


}Flag_Type;

typedef enum
{
	RESET,
	SET
}Status;


/*
 * =======================================================================================
 * 										Generic Functions
 * =======================================================================================
*/
static void I2C_GPIO_PINs(I2C_Periphral *I2Cx);

static void Generate_Start(I2C_Periphral *I2Cx, Start_Cond Start);

static void Generate_Stop(I2C_Periphral *I2Cx);

static Status Get_Status(I2C_Periphral *I2Cx, Flag_Type Flag);





/*
 * =======================================================================================
 * 										APIs
 * =======================================================================================
*/


/*The following is the required sequence in master mode.
* 1- Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
* 2- Configure the clock control registers
* 3- Configure the rise time register
* 4- Program the I2C_CR1 register to enable the peripheral
*/

void I2C_INIT(I2C *I2C_Conf)
{
	uint32_t PCLK = 0, FREQ = 0;
	if (I2C_Conf->I2Cx == I2C1)
	{
		RCC_I2C1_CLK_EN;
		I2C_GPIO_PINs(I2C1);
	}
	else if (I2C_Conf->I2Cx == I2C2)
	{
		RCC_I2C2_CLK_EN;
		I2C_GPIO_PINs(I2C2);
	}

	/*1- Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
	* Bits 5:0 FREQ[5:0]: Peripheral clock frequency in CR2
	* The minimum allowed frequency is 2 MHz,
	* the maximum frequency is limited by the maximum APB frequency and cannot exceed
	* 50 MHz (peripheral intrinsic maximum limit).
	* 0b000000: Not allowed
	* 0b000001: Not allowed
	* 0b000010: 2 MHz
	* ...
	* 0b110010: 50 MHz
	* Higher than 0b100100: Not allowed
	*/
	PCLK = RCC_GetPCLK1Freq();
	FREQ = PCLK / 1000000;
	I2C_Conf->I2Cx->CR2 &= ~(0b111111 << FREQ_BITS_Pos);
	I2C_Conf->I2Cx->CR2 |= FREQ << FREQ_BITS_Pos;


	/*2- Configure the clock control registers
	* The CCR register must be configured only when the I2C is disabled (PE = 0).
	* Thigh = CCR * Tpclk
	* Ti2c = Thigh + Tlow
	* Thigh = Tlow (Duty cycle 50%)
	* CCR = Thigh / Tpclk || (Ti2c/2) / Tpclk || Fpclk / (Fi2c*2)
	* CCR = Fpclk / (Fi2c*2)
	*/

	// Clear PE Bit to config CCR Reg
	I2C_Conf->I2Cx->CR1 &= ~(0b1 << PE_BIT_Pos);

	if (I2C_Conf->Mode == SM_Mode)
	{
		// Enable SM_Mode
		I2C_Conf->I2Cx->CR1 &= ~(0b1 << F_S_BIT_Pos);
		I2C_Conf->I2Cx->CCR = ((PCLK / (I2C_Conf->SCL_Speed << 1)) & 0xFFF) << CRR_BITS_Pos;
	}
	else
	{
		// To Do
	}

	/*3- Configure the rise time register
	 * FREQ + 1
	 */
	I2C_Conf->I2Cx->TRISE = (((I2C_Conf->I2Cx->CR2 << FREQ_BITS_Pos) & 0x3F) + 1 ) & 0x3F << TRISE_BITS_Pos;

	// Set PE Bit to Enable I2C Periphral
	I2C_Conf->I2Cx->CR1 |= (0b1 << PE_BIT_Pos);
}

void I2C_Slave_Init (I2C *I2C_Conf, Slave *Address)
{
	if (Address->Mode == Bit_7)
	{
		I2C_Conf->I2Cx->OAR1 &= ~(0b1 << ADD_MODE_BIT_POS);
		if (Address->Dual == ENABLE)
		{
			I2C_Conf->I2Cx->OAR2 |= (0b1 << ENDUAL_BIT_POS);
			I2C_Conf->I2Cx->OAR2 |= ((Address->Address2 & (0x7F))  << ADD2_BITS_POS);
		}
		I2C_Conf->I2Cx->OAR1 |= ((Address->Address1 & (0x7F)) << ADD_1_7_BITS_POS);
	}
	else if (Address->Mode == Bit_10)
	{
//		To Do
	}
}



void I2C_Master_Write(I2C *I2C_Conf, Address_Mode Mode, uint16_t devAddr, uint8_t *data, uint32_t dataLength, Stop_Cond Stop,Start_Cond Start)
{
//	Generate Start According to Start_Condition (Normal Start with Stop or Repeated Start )
	Generate_Start(I2C_Conf->I2Cx, Start);

	if (Mode == Bit_7)
	{
		uint32_t i = 0;
//		Polling Untill Start Bit Is generated Succesfully
//		EV5: SB=1, cleared by reading SR1 register followed by writing DR register with Address.
		while(!Get_Status(I2C_Conf->I2Cx, EV5));

		// Send Slave Address on Bus
		I2C_Conf->I2Cx->DR =  ((devAddr  & 0x7F) << 1) | Writing_Operation;

//		Polling till Address Sent
//		EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
		while(!Get_Status(I2C_Conf->I2Cx, EV6));
//		Polling Till DR empty
//		EV8_1: TxE=1, shift register empty, data register empty, write Data1 in DR.
		while(!Get_Status(I2C_Conf->I2Cx, EV8_1));

		for (i = 0; i < dataLength; i++)
		{
			I2C_Conf->I2Cx->DR =  *data;
			while(!Get_Status(I2C_Conf->I2Cx, EV8));
			data++;
		}
//		EV8_2: TxE=1, BTF = 1, Program Stop request. TxE and BTF are cleared by hardware by the Stop condition
//		(Option to Ensure that there is no Data in DR) (I Will not terminate For loop till I Send all data )
//		while(!Get_Status(I2C_Conf->I2Cx, EV8_2))

//		Generate Stop Bit if Start isn`t Repeated
		if (Stop != Without_Stop)
		{
			Generate_Stop(I2C_Conf->I2Cx);
		}
	}
	/* To Do In Future 10 Bit Address Mode */
	else if (Mode == Bit_10)
	{
//		To Do
	}
}


void I2C_Master_Read(I2C *I2C_Conf, Address_Mode Mode, uint16_t devAddr, uint8_t *dataOut, uint32_t dataLength, Stop_Cond Stop,Start_Cond Start)
{
//	Enable Ack Bit
	I2C_Conf->I2Cx->CR1 |= 0b1 << ACK_BIT_POS;

	Generate_Start(I2C_Conf->I2Cx, Start);

	if (Mode == Bit_7)
	{
		while(!Get_Status(I2C_Conf->I2Cx, EV5));

//		Set Device Address at DR Reg
		I2C_Conf->I2Cx->DR |= ((devAddr  & 0x7F) << 1);
//		Clearing First Bit Reading Operation
		I2C_Conf->I2Cx->DR &= ~(0b1 << 0);

//		Polling till Address Sent
//		EV6: ADDR=1, cleared by reading SR1 register followed by reading SR2.
		while(!Get_Status(I2C_Conf->I2Cx, EV6));

		if (dataLength)
		{
			for (uint32_t i = 0; i < dataLength; i++)
			{
//				EV7: RxNE = 1 Cleared by Reading DR Reg
				while(!Get_Status(I2C_Conf->I2Cx, EV6));
				*dataOut = I2C_Conf->I2Cx->DR;
				dataOut++;
			}
		}

//		Disable Ack Bit
		I2C_Conf->I2Cx->CR1 &= ~(0b1 << ACK_BIT_POS);

		if (Stop != Without_Stop)
		{
			Generate_Stop(I2C_Conf->I2Cx);
		}
	}
	/* To Do In Future 10 Bit Address Mode */
	else if (Mode == Bit_10)
	{
//		To Do
	}

}


void I2C_deINIT(I2C *I2C_Conf)
{
	if (I2C_Conf->I2Cx == I2C1)
	{
		RCC_I2C1_CLK_DIS;
		NVIC_Disable_ER_I2C1;
		NVIC_Disable_EV_I2C1;
	}
	else if (I2C_Conf->I2Cx == I2C2)
	{
		RCC_I2C2_CLK_DIS;
		NVIC_Disable_ER_I2C2;
		NVIC_Disable_EV_I2C2;
	}
}

/*
 * =======================================================================================
 * 										Generic Functions
 * =======================================================================================
*/


static void I2C_GPIO_PINs(I2C_Periphral *I2Cx)
{
	// Setting Pins from recommended As Mentioned in Data Sheet
	GPIO_PinConfig_t I2C_Pins;
	if (I2Cx == I2C1)
	{
		//SCL : PB6
		I2C_Pins.GPIOx = GPIOB;
		I2C_Pins.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		I2C_Pins.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
		I2C_Pins.GPIO_PinNumber = GPIO_PIN6;
		GPIO_INIT(&I2C_Pins);

		//SDA : PB7
		I2C_Pins.GPIOx = GPIOB;
		I2C_Pins.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		I2C_Pins.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
		I2C_Pins.GPIO_PinNumber = GPIO_PIN7;
		GPIO_INIT(&I2C_Pins);
	}
	else if (I2Cx == I2C2)
	{
		//SCL : PB10
		I2C_Pins.GPIOx = GPIOB;
		I2C_Pins.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		I2C_Pins.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
		I2C_Pins.GPIO_PinNumber = GPIO_PIN10;
		GPIO_INIT(&I2C_Pins);

		//SDA : PB11
		I2C_Pins.GPIOx = GPIOB;
		I2C_Pins.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
		I2C_Pins.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
		I2C_Pins.GPIO_PinNumber = GPIO_PIN11;
		GPIO_INIT(&I2C_Pins);
	}
}


static void Generate_Start(I2C_Periphral *I2Cx, Start_Cond Start)
{
	if (Start != Repeated)
	{
		// Polling on the bus Untill The Bus not busy
		while( !(Get_Status(I2Cx, Busy)) );
	}

//	Bit 8 START: Start generation
//	This bit is set and cleared by software and cleared by hardware when start is sent or PE=0.
//	In Master Mode:
//	0: No Start generation
//	1: start generation

	I2Cx->CR1 |= 0b1 << START_BIT_Pos;

}

static void Generate_Stop(I2C_Periphral *I2Cx)
{
//	Bit 9 STOP: Stop generation
//	The bit is set and cleared by software, cleared by hardware when a Stop condition is
//	detected, set by hardware when a timeout error is detected.
//	In Master Mode:
//	0: No Stop generation.
//	1: Stop generation after the current byte transfer or after the current Start condition is sent

	I2Cx->CR1 |= 0b1 << STOP_BIT_Pos;
}

static Status Get_Status(I2C_Periphral *I2Cx, Flag_Type Flag)
{
	Status bitStatus = RESET;
	uint32_t dummyRead;
	switch(Flag)
	{
	case Busy :
	{
		if (I2Cx->SR2 & 0b1 << BUSY_BIT_Pos)
			bitStatus = SET;
		else
			bitStatus = RESET;
		break;
	}
	case EV5:
	{
		if (I2Cx->SR1 & 1 << SB_BIT_Pos)
		{
			bitStatus = SET;
			dummyRead = I2Cx->SR1;
		}
		else
			bitStatus = RESET;
		break;
	}
	case EV6:
	{
		if (I2Cx->SR1 & (0b1 << ADDR_BIT_Pos))
		{
			bitStatus = SET;
			dummyRead = I2Cx->SR1;
			dummyRead = I2Cx->SR2;
		}
		else
			bitStatus = RESET;
		break;
	}
	case EV8_1:
	{
		if ( (I2Cx->SR1 & (1 << TxE_BIT_Pos)) && (I2Cx->SR2 & (0b111 << 0)) )
			bitStatus = SET;
		else
			bitStatus = RESET;
		break;
	}

	case EV8:
	{
		if (I2Cx->SR1 & (1 << TxE_BIT_Pos))
			bitStatus = SET;
		else
			bitStatus = RESET;
		break;
	}
	case EV8_2:
	{
		if ( (I2Cx->SR1 & (1 << TxE_BIT_Pos)) && ((I2Cx->SR1 & 1) << BTF_BIT_Pos) )
			bitStatus = SET;
		else
			bitStatus = RESET;
		break;
	}
	case EV7:
	{
		if (I2Cx->SR1 & 1 << RxNE_BIT_Pos)
			bitStatus = SET;
		else
			bitStatus = RESET;
		break;
	}

	}

	return bitStatus;
}
