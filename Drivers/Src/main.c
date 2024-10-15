/**
 ******************************************************************************
 * @File           : main.c
 * @Author         : Abdelrhman Marzoq
 * @Brief          : Main program body
 ******************************************************************************
**/

#include "../STM32F103C6_MCAL_Drivers/UART/STM32F103C6_UART_Driver.h"
#include "../STM32F103C6_MCAL_Drivers/GPIO/STM32F103C6_GPIO_Driver.h"

static void myWait(volatile int ms)
{
	for (volatile int i = 0; i < ms; i++)
	{
		for (volatile int j = 0; j < 255; j++);
	}
}


int main(void)
{
	RCC_GPIOA_CLK_EN;

	UART uart1;
	uart1.BaudRate = UART_BaudRate_9600;
	uart1.UARTx = USART1;
	uart1.StopBits = UART_StopBits_1;
	uart1.IRQ_Enable = UART_IRQ_Enable_NONE;
	uart1.P_IRQ_CallBack = NULL;
	uart1.Parity = UART_PARITY_DIS;
	uart1.USART_Mode = USART_MODE_TX_RX;

	USART_INIT(&uart1);


	uint8_t ch[] = "Ahmed \r";
	GPIO_PinConfig_t PinLED;

	PinLED.GPIOx = GPIOA;
	PinLED.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	PinLED.GPIO_OUTPUT_Speed = GPIO_SPEED_50M;
	PinLED.GPIO_PinNumber = GPIO_PIN5;

	GPIO_INIT(&PinLED);
	GPIO_WRITE_PIN(&PinLED, LOW);
	while(1)
	{

	}
}




