/*
 * STM32F103C6_I2C_Driver.c
 *
 *  Created on: Jan 27, 2025
 *      Author: Abdelrhman Marzoq
 */




#include "STM32F103C6_I2C_Driver.h"

static void GPIO_PINS_INIT(I2C_Periphral *I2Cx);


#define SET_BIT(REG, BIT_NO) 		(REG |= (1 << BIT_NO))
#define CLEAR_BIT(REG, BIT_NO) 		(REG &= (~(1 << BIT_NO)))
#define READ_BIT(REG, BIT_NO)		((REG >> BIT_NO) & 1)





static void Generate_START(I2C * hi2c, START_t START_Cond);
static void Generate_STOP(I2C * hi2c, STOP_t STOP_Cond);
static void Master_Transmit_Address(I2C * hi2c, unsigned int DevAddress ,AddressMode Mode, W_R COMMAND);



void I2C_INIT(I2C * hi2c)
{
	unsigned int CCR, PCLK = 0, FREQ = 0;


	GPIO_PINS_INIT(hi2c->I2Cx);

	// Disable Peripheral
	CLEAR_BIT(hi2c->I2Cx->CR1, PE_BIT_Pos);



	/************************************** CR1 Reg **************************************/
	if (hi2c->SCL_Streach_EN == ENABLE)
		CLEAR_BIT(hi2c->I2Cx->CR1, NOSTRETCH_BIT_POS);
	else if (hi2c->SCL_Streach_EN == DISABLE)
		SET_BIT(hi2c->I2Cx->CR1, NOSTRETCH_BIT_POS);


	/************************************** CR2 Reg **************************************/
	PCLK = RCC_GetPCLK1Freq();
	FREQ = PCLK / 1000000;
	// Min CLOCK = 2MHz to EN I2C
	if (FREQ < 2) return;

	// Clearin FREQ Bits
	hi2c->I2Cx->CR2 &= ~(0b111111 << FREQ_BITS_Pos);
	// Set FREQ OF SCL
	hi2c->I2Cx->CR2 |= FREQ << FREQ_BITS_Pos;

	if (hi2c->IRQ_EN == DISABLE)
	{
		// Disable IRQ
		CLEAR_BIT(hi2c->I2Cx->CR2, ITERREN_BIT_Pos);
		CLEAR_BIT(hi2c->I2Cx->CR2, ITEVTEN_BIT_Pos);
		CLEAR_BIT(hi2c->I2Cx->CR2, ITBUFEN_BIT_Pos);
	}


	/************************************** OAR1 Reg **************************************/
	/**************************************    &&    **************************************/
	/************************************** OAR2 Reg **************************************/

	// Init 7 Bit or 10 Bit Address in Slave Mode
	if (hi2c->Add_Mode == _7BIT_ADD)
	{
		CLEAR_BIT(hi2c->I2Cx->OAR1, ADD_MODE_BIT_POS);
		hi2c->I2Cx->OAR1 |= ( (hi2c->OwnAddress1 & 0x7F) << ADD_1_7_BITS_POS);
		if (hi2c->Dual_EN == ENABLE)
		/* Set Dual Address  for 7 Bit Addressing Only*/
		{
			SET_BIT(hi2c->I2Cx->OAR2, ENDUAL_BIT_POS);
			hi2c->I2Cx->OAR2 |= ( (hi2c->OwnAddress2 & 0x7F) << ADD2_BITS_POS);
		}
	}
	else if (hi2c->Add_Mode == _10BIT_ADD)
	{
		SET_BIT(hi2c->I2Cx->OAR1, ADD_MODE_BIT_POS);
		hi2c->I2Cx->OAR1 |= ( (hi2c->OwnAddress1 & 0x3FF) << ADD_10_BITS_POS);
	}

	/************************************** CRR Reg **************************************/
	if (hi2c->SF_Mode == FM_MODE)
	{
		SET_BIT(hi2c->I2Cx->CCR, F_S_BIT_Pos);
		// To Do Init
	}
	else if (hi2c->SF_Mode == SM_MODE)
	{
		CLEAR_BIT(hi2c->I2Cx->CCR, F_S_BIT_Pos);
		CCR = PCLK/( 2 * hi2c->SCL_Speed);
		hi2c->I2Cx->CCR |= ((CCR & 0x1FF) << CRR_BITS_Pos);
	}

	/************************************** TRISE Reg **************************************/

	hi2c->I2Cx->TRISE |= ( (FREQ + 1) << TRISE_BITS_Pos);

	// Enable Peripheral
	SET_BIT(hi2c->I2Cx->CR1, PE_BIT_Pos);
}


void I2C_ActivateIRQ_Bit(I2C * hi2c, unsigned int IRQ_Activate)
{
	if ( (IRQ_Activate & BUFFER_IRQ_EN) == BUFFER_IRQ_EN) SET_BIT(hi2c->I2Cx->CR2, ITERREN_BIT_Pos);

	if ( (IRQ_Activate & EVENT_IRQ_EN) == EVENT_IRQ_EN) SET_BIT(hi2c->I2Cx->CR2, ITEVTEN_BIT_Pos);

	if ( (IRQ_Activate & ERROR_IRQ_EN) == ERROR_IRQ_EN) SET_BIT(hi2c->I2Cx->CR2, ITBUFEN_BIT_Pos);
}

void I2C_Master_Transmit(I2C * hi2c, unsigned int DevAddress, SS_State *Start_Stop,AddressMode Mode, unsigned char *payload, unsigned int lenght)
{
	// Temp VAR
	volatile unsigned int dummy_reg;

	// Check if The Peripheral in Disable or Enable
	if (!READ_BIT(hi2c->I2Cx->CR1, PE_BIT_Pos))
	{
		// Enable Peripheral
		SET_BIT(hi2c->I2Cx->CR1, PE_BIT_Pos);
	}

	/***************************** Start of First State **********************************/
	// Generate Start
	Generate_START(hi2c, Start_Stop->START_Cond);
	// Polling Untill Start Send
	while (!READ_BIT(hi2c->I2Cx->SR1, SB_BIT_Pos));

	/******************************* End of First State **********************************/

	/******************************* Start of Second State *******************************/
	// Clearing SB Bit
	// Reading SR1
	// Writing on DR Reg with Data
	dummy_reg = hi2c->I2Cx->SR1;

	Master_Transmit_Address(hi2c, DevAddress, Mode, WRITE);

	// Polling Untill Address Send
	while (!READ_BIT(hi2c->I2Cx->SR1, ADDR_BIT_Pos));
	/******************************* End of Second State *********************************/

	/******************************* Start of Third State *******************************/
	// 									Send Data

	// Clear ADDR Bit Reading Both SR1,SR2 Reg
	dummy_reg = hi2c->I2Cx->SR1 | hi2c->I2Cx->SR2;

	for (unsigned int Counter = 0; Counter < lenght; Counter++)
	{
		while(!READ_BIT(hi2c->I2Cx->SR1, TxE_BIT_Pos));
		hi2c->I2Cx->DR = payload[Counter];
	}
	/******************************* End of Third State **********************************/


	/******************************* Start of Fourth State *******************************/

	if (Start_Stop->STOP_Cond == WithSTOP)
	{
		// Polling on Last EVENT
		while ( !READ_BIT(hi2c->I2Cx->SR1, TxE_BIT_Pos) );
		while ( !READ_BIT(hi2c->I2Cx->SR1, BTF_BIT_Pos) );
	}

	Generate_STOP(hi2c, Start_Stop->STOP_Cond);


	/******************************* End of Foruth State   *******************************/

}


void I2C_Master_Recieve(I2C * hi2c, unsigned int DevAddress, SS_State *Start_Stop,AddressMode Mode, unsigned char *payload, unsigned int lenght)
{
	// Temp VAR
	volatile unsigned int dummy_reg;

	// Enable ACK Bit
	SET_BIT(hi2c->I2Cx->CR1, ACK_BIT_POS);


	// Check if The Peripheral in Disable or Enable
	if (!READ_BIT(hi2c->I2Cx->CR1, PE_BIT_Pos))
	{
		// Enable Peripheral
		SET_BIT(hi2c->I2Cx->CR1, PE_BIT_Pos);
	}

	/***************************** Start of First State **********************************/
	// Generate Start
	Generate_START(hi2c, Start_Stop->START_Cond);
	// Polling Untill Start Send
	while (!READ_BIT(hi2c->I2Cx->SR1, SB_BIT_Pos));

	/******************************* End of First State **********************************/

	/******************************* Start of Second State *******************************/
	// Clearing SB Bit
	// Reading SR1
	// Writing on DR Reg with Data
	dummy_reg = hi2c->I2Cx->SR1;

	Master_Transmit_Address(hi2c, DevAddress, Mode, READ);

	// Polling Untill Address Send
	while (!READ_BIT(hi2c->I2Cx->SR1, ADDR_BIT_Pos));
	/******************************* End of Second State *********************************/

	/******************************* Start of Third State *******************************/
	// 									Recieve Data

	// Clear ADDR Bit Reading Both SR1,SR2 Reg
	dummy_reg = hi2c->I2Cx->SR1 | hi2c->I2Cx->SR2;

	for (unsigned int Counter = 0; Counter < (lenght-1); Counter++)
	{
		while(!READ_BIT(hi2c->I2Cx->SR1, 6));
		payload[Counter] = hi2c->I2Cx->DR;
	}

	// Disable ACK Bit (NACK)
	CLEAR_BIT(hi2c->I2Cx->CR1, ACK_BIT_POS);

	// Recieve the data with NACK Sending
	while(!READ_BIT(hi2c->I2Cx->SR1, 6));
	payload[lenght-1] = hi2c->I2Cx->DR;

	Generate_STOP(hi2c, Start_Stop->STOP_Cond);



}


void I2C_Slave_Transmit(I2C * hi2c)
{

}


void I2C_Slave_Recieve(I2C * hi2c)
{

}




static void GPIO_PINS_INIT(I2C_Periphral *I2Cx)
{

	GPIO_PinConfig_t I2C_PINS;

	// Common Configuration between I2C1,I2C2
	I2C_PINS.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
	I2C_PINS.GPIO_MODE = GPIO_MODE_OUTPUT_AF_OD;
	I2C_PINS.GPIOx = GPIOB;
	RCC_GPIOB_CLK_EN;
	if (I2Cx == I2C1)
	{
		// Enable Clock For I2C1
		RCC_I2C1_CLK_EN;

		// SCL : PB6
		I2C_PINS.GPIO_PinNumber = GPIO_PIN6;
		GPIO_INIT(&I2C_PINS);

		// SDA : PB7
		I2C_PINS.GPIO_PinNumber = GPIO_PIN7;
		GPIO_INIT(&I2C_PINS);
	}
	else if (I2Cx == I2C2)
	{
		// Enable Clock For I2C2
		RCC_I2C2_CLK_EN;

		// SCL : PB6
		I2C_PINS.GPIO_PinNumber = GPIO_PIN10;
		GPIO_INIT(&I2C_PINS);

		// SDA : PB7
		I2C_PINS.GPIO_PinNumber = GPIO_PIN11;
		GPIO_INIT(&I2C_PINS);
	}
}

static void Generate_START(I2C * hi2c, START_t START_Cond)
{

	if (START_Cond == START)
	{
		// Polling Untill Bus be Free to win arbitration
		while(READ_BIT(hi2c->I2Cx->SR2, BUSY_BIT_Pos));
	}
	// Send START Bit
	SET_BIT(hi2c->I2Cx->CR1, START_BIT_Pos);
}


static void Master_Transmit_Address(I2C * hi2c, unsigned int DevAddress ,AddressMode Mode, W_R COMMAND)
{
	if (Mode == _7BIT_ADD)
	{
		hi2c->I2Cx->DR = (DevAddress << 1 | COMMAND);
	}
	// Need Enhancment
	else if (Mode == _10BIT_ADD)
	{
		unsigned int dummy_reg;
		if (COMMAND == WRITE)
		{
			unsigned char TempAddress = DevAddress;
			TempAddress = ((DevAddress >> 8) & 0b11);
			hi2c->I2Cx->DR = RESERVED_FRAME_10BIT_ADDRESS | (TempAddress << 1) | COMMAND;
			// Wait Untill Header Send Reading
			while(!READ_BIT(hi2c->I2Cx->SR1, ADD10_BIT_Pos));
			// Clear ADD10 Bit Reading SR1 & Writing DR
			dummy_reg = hi2c->I2Cx->SR1;
			hi2c->I2Cx->DR = (DevAddress & 0xFF);
		}
		else if (COMMAND == READ)
		{

			unsigned char TempAddress = DevAddress;
			// Sending Header
			TempAddress = ((DevAddress >> 8) & 0b11);
			hi2c->I2Cx->DR = RESERVED_FRAME_10BIT_ADDRESS | (TempAddress << 1) | COMMAND;
			// Wait Untill Header Send Reading
			while(!READ_BIT(hi2c->I2Cx->SR1, ADD10_BIT_Pos));
			// Clear ADD10 Bit Reading SR1 & Writing DR (Address)
			dummy_reg = hi2c->I2Cx->SR1;
			hi2c->I2Cx->DR = (DevAddress & 0xFF);

			// Send Repeated Start
			Generate_START(hi2c, RepeatedSTART);
			// Polling Untill Repeated Start Send
			while (!READ_BIT(hi2c->I2Cx->SR1, SB_BIT_Pos));

			// Sending Header
			TempAddress = ((DevAddress >> 8) & 0b11);
			hi2c->I2Cx->DR = RESERVED_FRAME_10BIT_ADDRESS | (TempAddress << 1) | COMMAND;
		}

	}
}

static void Generate_STOP(I2C * hi2c, STOP_t STOP_Cond)
{
	if (STOP_Cond == WithSTOP)
	{

		SET_BIT(hi2c->I2Cx->CR1, STOP_BIT_Pos);
	}
}




