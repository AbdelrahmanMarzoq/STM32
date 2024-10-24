/**
 ******************************************************************************
 * @File           : STM32F103C6_SPI_Driver.h
 * @Author         : Abdelrhman Marzoq
 * @Brief          : SPI Driver
 *
 ******************************************************************************
**/


#include "STM32F103C6_SPI_Driver.h"



/*
 * =======================================================================================
 * 										Generic Variables
 * =======================================================================================
*/



/*
 * =======================================================================================
 * 										Generic Functions
 * =======================================================================================
*/

static void SPI_GPIO_PINs(SPI *SPI_Conf);



/*
 * =======================================================================================
 * 										APIs
 * =======================================================================================
*/

void SPI_MASTER_INIT(SPI *SPI_Conf)
{
	uint16_t tempCR1 = 0;
	uint8_t  tempCR2  = 0;

	if (SPI_Conf->SPIx == SPI1)
	{
		RCC_SPI1_CLK_EN;
	}
	else if (SPI_Conf->SPIx == SPI2)
	{
		RCC_SPI2_CLK_EN;
	}

	// Enable Periphral
	tempCR1 |= (1 << SPE_BIT_Pos);

	// Identify Master Or Slave
	if (SPI_Conf->Master_Slave_Mode == SPI_Slave)
		tempCR1 &= ~(1 << MSTR_BIT_Pos);
	else if (SPI_Conf->Master_Slave_Mode == SPI_Master)
		tempCR1 |= (1 << MSTR_BIT_Pos);

	//Clearing LSBFIRST Bit this Mean in Frame Will Send MSB First
	tempCR1 &= ~(1 << LSBFIRST_BIT_Pos);

	// Identify Frame Size of Data Transmit and recieved
	if ( SPI_Conf->Frame_Size == SPI_BIT_8 )
		tempCR1 &= ~(1 << DFF_BIT_Pos);    	// Frame of Data will be 8  Bit
	else if (SPI_Conf->Frame_Size == SPI_BIT_16)
		tempCR1 |= (1 << DFF_BIT_Pos);		// Frame of Data will be 16 Bit

	// Set Freq Of SCLK
	tempCR1 |= SPI_Conf->Prescaller << BR_BITS_Pos;

	// Idle CLK Is One
	tempCR1 |= 1 << CPOL_BIT_Pos;

	// The Second Edge of CLK Capture The Data
	tempCR1 |= 1 << CPHA_BIT_Pos;

	// Init SPI Communication Mode
	if (SPI_Conf->Communiaction_Mode == Half_Duplex)
	{
		// Not Supported (To Do)
		// Half Duplex interaccte with MISO Pin
		// Bidirectional Recieve Only
		tempCR1 |= 1 << BIDIMODE_BIT_Pos;
		tempCR1 &= ~(1 << BIDIOE_BIT_Pos);
	}
	else if (SPI_Conf->Communiaction_Mode == Full_Duplex)
	{
		// Full Duplex Mode Enable MOSI and MISO
		tempCR1 &= ~(1 << BIDIMODE_BIT_Pos);
		tempCR1 &= ~(1 << RXONLY_BIT_Pos);
	}

	// NSS SW (Not Supported HW)
	tempCR1 |= 1 << SSM_BIT_Pos;



	// En Or Dis IRQ
	if (SPI_Conf->IRQ != SPI_IRQ_NONE)
	{
		tempCR2 |= SPI_Conf->IRQ;
		if (SPI_Conf->SPIx == SPI1)
			NVIC_Enable_SPI1;
		else if (SPI_Conf->SPIx == SPI2)
			NVIC_Enable_SPI2;
	}
	else
	{
		if (SPI_Conf->SPIx == SPI1)
			NVIC_Disable_SPI1;
		else if (SPI_Conf->SPIx == SPI2)
			NVIC_Disable_SPI2;
	}


	SPI_GPIO_PINs(SPI_Conf);


	// Upload Configuration To CR1 and CR2
	SPI_Conf->SPIx->CR1 = (uint16_t)tempCR1;
	SPI_Conf->SPIx->CR2 = (uint16_t)tempCR2;
	tempCR1 |= (1 << SPE_BIT_Pos);
	SPI_Conf->SPIx->CR1 = (uint16_t)tempCR1;

}

void SPI_RX(SPI *SPI_Conf, uint16_t *RX_Buffer)
{

	// Pooling Untill Reccieve Buffer not empty
	while (! SPI_Conf->SPIx->SR & 1 << RXNE_BIT_Pos);

	*RX_Buffer = SPI_Conf->SPIx->DR;
}



void SPI_TX(SPI *SPI_Conf, uint16_t *TX_Buffer)
{


	// Pooling Untill Transmit Buffer empty
	while (! SPI_Conf->SPIx->SR & 1 << TXE_BIT_Pos);

	SPI_Conf->SPIx->DR = *TX_Buffer;
}


void SPI_RX_TX(SPI *SPI_Conf, uint16_t *TX_RX_Buffer)
{
	// Pooling Untill Transmit Buffer empty
	while (! SPI_Conf->SPIx->SR & 1 << TXE_BIT_Pos);

	SPI_Conf->SPIx->DR = *TX_RX_Buffer;

	// Pooling Untill Reccieve Buffer not empty
	while (! SPI_Conf->SPIx->SR & 1 << RXNE_BIT_Pos);

	*TX_RX_Buffer = SPI_Conf->SPIx->DR;
}


void SPI_deINIT(SPI *SPI_Conf)
{

	if (SPI_Conf->SPIx == SPI1)
	{
		RCC_SPI1_CLK_DIS;
		NVIC_Disable_SPI1;
	}
	else if (SPI_Conf->SPIx == SPI2)
	{
		RCC_SPI2_CLK_DIS;
		NVIC_Disable_SPI2;
	}
}



/*
 * =======================================================================================
 * 										Generic Functions
 * =======================================================================================
*/


static void SPI_GPIO_PINs(SPI *SPI_Conf)
{
	GPIO_PinConfig_t SPI_PIN;
	if (SPI_Conf->SPIx == SPI1)
	{
		// PA5 SCK
		// PA6 MISO
		// PA7 MOSI
		SPI_PIN.GPIOx = GPIOA;
		if (SPI_Conf->Master_Slave_Mode == SPI_Master)
		{
			// Init CLK Pin
			SPI_PIN.GPIO_PinNumber = GPIO_PIN5;
			SPI_PIN.GPIO_OUTPUT_Speed = GPIO_SPEED_10M;
			SPI_PIN.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			GPIO_INIT(&SPI_PIN);


			// Init MISO Pin
			if (SPI_Conf->Communiaction_Mode == Full_Duplex)
			{
				SPI_PIN.GPIO_PinNumber = GPIO_PIN6;
				SPI_PIN.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				GPIO_INIT(&SPI_PIN);
			}
			else if (SPI_Conf->Communiaction_Mode == Half_Duplex)
			{
				// To Do
			}

			// Init MOSI Pin
			if (SPI_Conf->Communiaction_Mode == Full_Duplex)
			{
				SPI_PIN.GPIO_PinNumber = GPIO_PIN7;
				SPI_PIN.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				SPI_PIN.GPIO_OUTPUT_Speed = GPIO_SPEED_10M;
				GPIO_INIT(&SPI_PIN);
			}
			else if (SPI_Conf->Communiaction_Mode == Half_Duplex)
			{
				// To Do
			}

		}
		else if (SPI_Conf->Master_Slave_Mode == SPI_Slave)
		{
			// Init CLK Pin
			SPI_PIN.GPIO_PinNumber = GPIO_PIN5;
			SPI_PIN.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			GPIO_INIT(&SPI_PIN);


			// Init MISO Pin
			if (SPI_Conf->Communiaction_Mode == Full_Duplex)
			{
				SPI_PIN.GPIO_PinNumber = GPIO_PIN6;
				SPI_PIN.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				SPI_PIN.GPIO_OUTPUT_Speed = GPIO_SPEED_10M;
				GPIO_INIT(&SPI_PIN);
			}
			else if (SPI_Conf->Communiaction_Mode == Half_Duplex)
			{
				// To Do
			}

			// Init MOSI Pin
			if (SPI_Conf->Communiaction_Mode == Full_Duplex)
			{
				SPI_PIN.GPIO_PinNumber = GPIO_PIN7;
				SPI_PIN.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				GPIO_INIT(&SPI_PIN);
			}
			else if (SPI_Conf->Communiaction_Mode == Half_Duplex)
			{
				// To Do
			}
		}
	}
	else if (SPI_Conf->SPIx == SPI2)
	{
		// PB13 SCK
		// PB14 MISO
		// PB15 MOSI

		SPI_PIN.GPIOx = GPIOB;
		if (SPI_Conf->Master_Slave_Mode == SPI_Master)
		{
			// Init CLK Pin
			SPI_PIN.GPIO_PinNumber = GPIO_PIN13;
			SPI_PIN.GPIO_OUTPUT_Speed = GPIO_SPEED_10M;
			SPI_PIN.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
			GPIO_INIT(&SPI_PIN);


			// Init MISO Pin
			if (SPI_Conf->Communiaction_Mode == Full_Duplex)
			{
				SPI_PIN.GPIO_PinNumber = GPIO_PIN14;
				SPI_PIN.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				GPIO_INIT(&SPI_PIN);
			}
			else if (SPI_Conf->Communiaction_Mode == Half_Duplex)
			{
				// To Do
			}

			// Init MOSI Pin
			if (SPI_Conf->Communiaction_Mode == Full_Duplex)
			{
				SPI_PIN.GPIO_PinNumber = GPIO_PIN15;
				SPI_PIN.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				SPI_PIN.GPIO_OUTPUT_Speed = GPIO_SPEED_10M;
				GPIO_INIT(&SPI_PIN);
			}
			else if (SPI_Conf->Communiaction_Mode == Half_Duplex)
			{
				// To Do
			}

		}
		else if (SPI_Conf->Master_Slave_Mode == SPI_Slave)
		{
			// Init CLK Pin
			SPI_PIN.GPIO_PinNumber = GPIO_PIN13;
			SPI_PIN.GPIO_MODE = GPIO_MODE_INPUT_FLO;
			GPIO_INIT(&SPI_PIN);


			// Init MISO Pin
			if (SPI_Conf->Communiaction_Mode == Full_Duplex)
			{
				SPI_PIN.GPIO_PinNumber = GPIO_PIN14;
				SPI_PIN.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
				SPI_PIN.GPIO_OUTPUT_Speed = GPIO_SPEED_10M;
				GPIO_INIT(&SPI_PIN);
			}
			else if (SPI_Conf->Communiaction_Mode == Half_Duplex)
			{
				// To Do
			}

			// Init MOSI Pin
			if (SPI_Conf->Communiaction_Mode == Full_Duplex)
			{
				SPI_PIN.GPIO_PinNumber = GPIO_PIN15;
				SPI_PIN.GPIO_MODE = GPIO_MODE_INPUT_FLO;
				GPIO_INIT(&SPI_PIN);
			}
			else if (SPI_Conf->Communiaction_Mode == Half_Duplex)
			{
				// To Do
			}
		}
	}

}

