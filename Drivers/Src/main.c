/**
 ******************************************************************************
 * @File           : main.c
 * @Author         : Abdelrhman Marzoq
 * @Brief          : Main program body
 ******************************************************************************
**/

#include "../STM32F103C6_MCAL_Drivers/UART/STM32F103C6_UART_Driver.h"


static void myWait(volatile int ms)
{
	for (volatile int i = 0; i < ms; i++)
	{
		for (volatile int j = 0; j < 255; j++);
	}
}


int main(void)
{

	UART uart1;
	uart1.BaudRate = UART_BaudRate_9600;
	uart1.UARTx = USART1;
	uart1.StopBits = UART_StopBits_1;
	uart1.IRQ_Enable = UART_IRQ_Enable_NONE;
	uart1.P_IRQ_CallBack = NULL;
	uart1.Parity = UART_PARITY_DIS;
	uart1.USART_Mode = USART_MODE_TX_RX;
//
	USART_INIT(&uart1);

	uint8_t ch = 'A';
	while(1)
	{
		USART_RecieveData(&uart1, &ch);
		USART_SendData(&uart1, &ch);
	}
}




