/**
 ******************************************************************************
 * @File           : STM32F103C6_USART_Driver.c
 * @Author         : Abdelrhman Marzoq
 * @Brief          : USART Driver
 * 			Created on: Oct 7, 2024
 ******************************************************************************
**/

#include "STM32F103C6_UART_Driver.h"


static void USART_GPIO_TX_PINs(UART **UART_Conf);
static void USART_GPIO_RX_PINs(UART **UART_Conf);

void(* P_IRQ_CallBack[3])(void);




void USART_INIT(UART *UART_Conf)
{
	uint32_t PCLK, Mantissa, DIV;

	// Enable Clock for USART Periphral
	if (UART_Conf->UARTx == USART1)
		RCC_USART1_CLK_EN;
	else if (UART_Conf->UARTx == USART2)
		RCC_USART2_CLK_EN;
	else if (UART_Conf->UARTx == USART3)
		RCC_USART3_CLK_EN;



	//	Bit 13 UE: USART enable
	//	When this bit is cleared the USART prescalers and outputs are stopped and the end of the
	//	current
	//	byte transfer in order to reduce power consumption. This bit is set and cleared by software.
	//	0: USART prescaler and outputs disabled
	//	1: USART enable
	UART_Conf->UARTx->CR1 |= 1<<13;

	// Init TX & RX
	if (UART_Conf->USART_Mode == USART_MODE_RX)
	{
		//Enable RX and Disable TX on Bit RE and TE on CR1 Reg
		UART_Conf->UARTx->CR1 |= 1<<2;
		UART_Conf->UARTx->CR1 &= (~(1<<3));
		// Init Pins for USART
		USART_GPIO_RX_PINs(&UART_Conf);
	}
	else if (UART_Conf->USART_Mode == USART_MODE_TX)
	{
		//Enable TX and Disable RX on Bit RE and TE on CR1 Reg
		UART_Conf->UARTx->CR1 &= (~(1<<2));
		UART_Conf->UARTx->CR1 |= 1<<3;
		// Init Pins for USART
		USART_GPIO_TX_PINs(&UART_Conf);
	}
	else if (UART_Conf->USART_Mode == USART_MODE_TX_RX)
	{
		//Enable TX & RX on Bit RE and TE on CR1 Reg
		UART_Conf->UARTx->CR1 |= 1<<2;
		UART_Conf->UARTx->CR1 |= 1<<3;
		// Init Pins for USART
		USART_GPIO_RX_PINs(&UART_Conf);
		USART_GPIO_TX_PINs(&UART_Conf);
	}

	// Init Data Byte sending 8bit
	UART_Conf->UARTx->CR1 &= ~(1<<12);


	// Init Parity Feature
	if (UART_Conf->Parity == UART_PARITY_EN)
	{
		UART_Conf->UARTx->CR1 |= 1<<10;
		if (UART_Conf->Parity_Selection == UART_PARITY_EVEN)
		{
			UART_Conf->UARTx->CR1 &= (~(1<<9));
		}
		else if (UART_Conf->Parity_Selection == UART_PARITY_ODD)
		{
			UART_Conf->UARTx->CR1 |= 1<<9;
		}
	}
	else if (UART_Conf->Parity == UART_PARITY_DIS)
	{
		UART_Conf->UARTx->CR1 &= (~(1<<10));
	}

	// Init Stop Bit
	UART_Conf->UARTx->CR2 &= ~(0b11 << 12);
	if ( UART_Conf->StopBits == UART_StopBits_HALF )
	{
		UART_Conf->UARTx->CR2 |= 1<<12;
		UART_Conf->UARTx->CR2 &= (~(1<<13));
	}
	else if ( UART_Conf->StopBits == UART_StopBits_1 )
	{
		UART_Conf->UARTx->CR2 &= (~(0b11<<12));

	}
	else if ( UART_Conf->StopBits == UART_StopBits_1_HALF )
	{
		UART_Conf->UARTx->CR2 |= (0b11<<12);
	}
	else if ( UART_Conf->StopBits == UART_StopBits_2 )
	{
		UART_Conf->UARTx->CR2 &= (~(1<<12));
		UART_Conf->UARTx->CR2 |= 1<<13;
	}


	// Init IRQ For USART
	if ( UART_Conf->IRQ_Enable != UART_IRQ_Enable_NONE )
	{
		UART_Conf->UARTx->CR1 |= UART_Conf->IRQ_Enable;
		//Enable NVIC For USARTx IRQ
		if ( UART_Conf->UARTx == USART1 )
		{
			NVIC_Enable_USART1;
			P_IRQ_CallBack[0] = UART_Conf->P_IRQ_CallBack;
		}
		else if ( UART_Conf->UARTx == USART2 )
		{
			NVIC_Enable_USART2;
			P_IRQ_CallBack[1] = UART_Conf->P_IRQ_CallBack;
		}
		else if ( UART_Conf->UARTx == USART3 )
		{
			NVIC_Enable_USART3;
			P_IRQ_CallBack[2] = UART_Conf->P_IRQ_CallBack;
		}
	}
	else
	{
		UART_Conf->UARTx->CR1 &= ~(0b1111<<5);
		if ( UART_Conf->UARTx == USART1 )
			NVIC_Disable_USART1;
		else if ( UART_Conf->UARTx == USART2 )
			NVIC_Disable_USART2;
		else if ( UART_Conf->UARTx == USART3 )
			NVIC_Disable_USART3;
	}

	// Init BaudRate
	if ( UART_Conf->UARTx == USART1 )
	{
		PCLK = RCC_GetPCLK2Freq();
	}
	else
	{
		PCLK = RCC_GetPCLK1Freq();
	}

	// Implement BRR Reg as Refer in DataSheet
	Mantissa = PCLK / (16 * UART_Conf->BaudRate);
	DIV = (PCLK * 100) / (16 * UART_Conf->BaudRate);
	DIV -= (Mantissa * 100);
	DIV *= (16.0/100);
	UART_Conf->UARTx->BRR  = Mantissa << 4 | (DIV & 0xF);
}

void USART_SendData(UART *UART_Conf, uint8_t *TxBuffer)
{
	// Send String Byte by Byte
	while (*TxBuffer != '\0')
	{
		UART_Conf->UARTx->DR = *TxBuffer;
		// Wait untill DR Be on Shift Reg to load The next Byte
		while(!((UART_Conf->UARTx->SR) & (1<<7)));
		TxBuffer++;
	}
}

void USART_RecieveData(UART *UART_Conf, uint8_t *RxBuffer)
{
	// Wait untill Recieve Data using Polling Mechanism
	while(!((UART_Conf->UARTx->SR) & (1<<5)));

	// Recieve data at RxBuffer
	if (UART_Conf->Parity == UART_PARITY_DIS)
	{
		*RxBuffer = UART_Conf->UARTx->DR & 0xFF;
	}
	else
	{
		*RxBuffer = UART_Conf->UARTx->DR & 0x7F;
	}

	// Clear Bit to Recieve another Byte
	UART_Conf->UARTx->SR &= ~(1<<5);
}

static void USART_GPIO_TX_PINs(UART **UART_Conf)
{

	// Set Pins as Recommended for UART Periphral (TX)
	GPIO_PinConfig_t PinUSART;

	if ( (*UART_Conf)->UARTx == USART1 )
	{
		//PA9 TX
		PinUSART.GPIOx = GPIOA;
		PinUSART.GPIO_PinNumber = GPIO_PIN9;
		PinUSART.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		PinUSART.GPIO_OUTPUT_Speed = GPIO_SPEED_50M;
		GPIO_INIT(&PinUSART);
	}
	else if ( (*UART_Conf)->UARTx == USART2 )
	{
		//PA2 TX
		PinUSART.GPIOx = GPIOA;
		PinUSART.GPIO_PinNumber = GPIO_PIN2;
		PinUSART.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		PinUSART.GPIO_OUTPUT_Speed = GPIO_SPEED_50M;
		GPIO_INIT(&PinUSART);
	}
	else if ( ((*UART_Conf))->UARTx == USART3 )
	{
		//PB10 TX
		PinUSART.GPIOx = GPIOB;
		PinUSART.GPIO_PinNumber = GPIO_PIN10;
		PinUSART.GPIO_MODE = GPIO_MODE_OUTPUT_AF_PP;
		PinUSART.GPIO_OUTPUT_Speed = GPIO_SPEED_50M;
		GPIO_INIT(&PinUSART);
	}
}

static void USART_GPIO_RX_PINs(UART **UART_Conf)
{

	// Set Pins as Recommended for UART Periphral (RX)
	GPIO_PinConfig_t PinUSART;

	if ( (*UART_Conf)->UARTx == USART1 )
	{
		//PA10 RX
		PinUSART.GPIOx = GPIOA;
		PinUSART.GPIO_PinNumber = GPIO_PIN10;
		PinUSART.GPIO_MODE = GPIO_MODE_INPUT_AF;
		PinUSART.GPIO_OUTPUT_Speed = GPIO_SPEED_50M;
		GPIO_INIT(&PinUSART);
	}
	else if ( (*UART_Conf)->UARTx == USART2 )
	{
		//PA3 RX
		PinUSART.GPIOx = GPIOA;
		PinUSART.GPIO_PinNumber = GPIO_PIN3;
		PinUSART.GPIO_MODE = GPIO_MODE_INPUT_AF;
		PinUSART.GPIO_OUTPUT_Speed = GPIO_SPEED_50M;
		GPIO_INIT(&PinUSART);
	}
	else if ( (*UART_Conf)->UARTx == USART3 )
	{
		//PB11 RX
		PinUSART.GPIOx = GPIOB;
		PinUSART.GPIO_PinNumber = GPIO_PIN11;
		PinUSART.GPIO_MODE = GPIO_MODE_INPUT_AF;
		PinUSART.GPIO_OUTPUT_Speed = GPIO_SPEED_50M;
		GPIO_INIT(&PinUSART);
	}
}

void USART_deINIT(UART *UART_Conf)
{

	// Disable USART Periphral
	if ( UART_Conf->UARTx == USART1 )
	{
		RCC_USART1_CLK_DIS;
		NVIC_Disable_USART1;
	}
	else if ( UART_Conf->UARTx == USART2 )
	{
		RCC_USART2_CLK_DIS;
		NVIC_Disable_USART2;
	}
	else if ( UART_Conf->UARTx == USART3 )
	{
		RCC_USART3_CLK_DIS;
		NVIC_Disable_USART3;
	}
}

// ISR
void USART1_IRQHandler()
{
	P_IRQ_CallBack[0]();
}

void USART2_IRQHandler()
{
	P_IRQ_CallBack[1]();
}

void USART3_IRQHandler()
{
	P_IRQ_CallBack[2]();
}

