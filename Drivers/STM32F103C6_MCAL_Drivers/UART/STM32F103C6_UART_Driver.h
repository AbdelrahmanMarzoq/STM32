/**
 ******************************************************************************
 * @File           : STM32F103C6_USART_Driver.h
 * @Author         : Abdelrhman Marzoq
 * @Brief          : Driver Header file
 * 			Created on: Oct 7, 2024
 ******************************************************************************
**/

#ifndef _UART_DRIVER_H_
#define _UART_DRIVER_H_

#include "../STM32F103X6.h"
#include "../RCC/STM32F103C6_RCC_Driver.h"
#include "../GPIO/STM32F103C6_GPIO_Driver.h"


typedef struct
{
	USART_Periphral *UARTx;						// Specifies Which USART Use.
	// This parameter must be set based on @ref UARTx

	uint8_t   		USART_Mode;     			// Specifies the TX/RX Mode.
	// This parameter must be set based on @ref UART_Mode_define


	uint32_t  		BaudRate; 					// This member configures the UART communication baud rate
	// This parameter must be set based on @ref UART_BaudRate_define

	uint8_t 		Parity;					//Specifies the parity mode.
	//@ref UART_Parity_define

	uint8_t 		Parity_Selection;		//Specifies the parity mode.
	//@ref UART_Parity_Selection_define


	uint8_t 		StopBits;				//Specifies the number of stop bits transmitted
	//@ref UART_StopBits_define

	uint8_t			IRQ_Enable;				//enable or disable UART IRQ TX/RX
	//@ref UART_IRQ_Enable_define , you can select two or three parameters EX.UART_IRQ_Enable_TXE |UART_IRQ_Enable_TC


	void(* P_IRQ_CallBack)(void);			//Set the C Function() which will be called once the IRQ  Happen

}UART;



/**********************************  Reference Macros  *************************************/

//@ref UARTx
//USART1					For using USART1
//USART2					For using USART2
//USART3					For using USART3

//@ref UART_Mode_define
#define USART_MODE_RX              0 								//Set RE Bit in USART_CR1
#define USART_MODE_TX              1								//Set TE Bit in USART_CR1
#define USART_MODE_TX_RX           2		//Set TE & RE Bits in USART_CR1

//@ref UART_BaudRate_define

#define UART_BaudRate_2400                   2400
#define UART_BaudRate_9600                   9600
#define UART_BaudRate_19200                  19200
#define UART_BaudRate_57600                  57600
#define UART_BaudRate_115200                 115200
#define UART_BaudRate_230400                 230400
#define UART_BaudRate_460800                 460800
#define UART_BaudRate_921600                 921600
#define UART_BaudRate_2250000                2250000
#define UART_BaudRate_4500000                4500000

//@ref UART_Payload_Length_define
//USART_CR1
//Bit 12
//M : Word length
//This bit determines the word length. It is set or cleared by software.
//0: 1 Start bit, 8 Data bits, n Stop bit
//1: 1 Start bit, 9 Data bits, n Stop bit

#define UART_WORD_LENGTH_8					0
#define UART_WORD_LENGTH_9					1

//@ref UART_Parity_define
//USART_CR1
//Bit 10 PCE: Parity control enable
//This bit selects the hardware parity control (generation and detection). When the parity
//control is enabled, the computed parity is inserted at the MSB position (9th bit if M=1; 8th bit
//if M=0) and parity is checked on the received data. This bit is set and cleared by software.
//Once it is set, PCE is active after the current byte (in reception and in transmission).
//0: Parity control disabled
//1: Parity control enabled

#define UART_PARITY_DIS					0
#define UART_PARITY_EN					1

//@ref UART_Parity_Selection_define
//USART_CR1
//Bit 9 PS: Parity selection
//This bit selects the odd or even parity when the parity generation/detection is enabled (PCE
//bit set). It is set and cleared by software. The parity will be selected after the current byte.
//0: Even parity
//1: Odd parity

#define UART_PARITY_EVEN				0
#define UART_PARITY_ODD					1

//@ref UART_StopBits_define
//USART_CR2
//Bits 13:12 STOP: STOP bits
//These bits are used for programming the stop bits.
//00: 1 Stop bit
//01: 0.5 Stop bit
//10: 2 Stop bits
//11: 1.5 Stop bit

#define UART_StopBits_HALF                  0
#define UART_StopBits_1                     1
#define UART_StopBits_1_HALF                2
#define UART_StopBits_2                     3

//@ref UART_IRQ_Enable_define
#define UART_IRQ_Enable_NONE               	(uint32_t)(0)
#define UART_IRQ_Enable_TXE                 (uint32_t) (1<<7) //Transmit data register empty
#define UART_IRQ_Enable_TC                  ((uint32_t)(1<<6)) //Transmission complete
#define UART_IRQ_Enable_RXNEIE              (uint32_t) (1<<5) //Received data ready to be read & Overrun error detected
#define UART_IRQ_Enable_PE                  (uint32_t) (1<<8) //Parity error






/*
 * =======================================================================================
 * 										APIs
 * =======================================================================================
*/

/**================================================================
 * @Fn                 	- USART_INIT
 * @brief				- Init USART Periphral
 * @param [in]			- Pointer to Your UART
 *
 * @retval				- NONE
 * Note					- NONE
*/
void USART_INIT(UART *UART_Conf);

/**================================================================
 * @Fn                 	- USART_SendData
 * @brief				- Func that Send your Data
 * @param [in]			- Pointer to Your UART & Pointer to Data need to send
 *
 * @retval				- NONE
 * Note					- NONE
*/
void USART_SendData(UART *UART_Conf, uint8_t *TxBuffer);

/**================================================================
 * @Fn                 	- USART_RecieveData
 * @brief				- Func That Recieve Data
 * @param [in]			- Pointer to Your UART & Pointer to VAR store datat recieved
 *
 * @retval				- NONE
 * Note					- NONE
*/
void USART_RecieveData(UART *UART_Conf, uint8_t *RxBuffer);

/**================================================================
 * @Fn                 	- USART_deINIT
 * @brief				- De init UART
 * @param [in]			- Pointer to Your UART
 *
 * @retval				- NONE
 * Note					- NONE
*/
void USART_deINIT(UART *UART_Conf);






#endif

