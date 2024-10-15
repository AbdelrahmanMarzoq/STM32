/**
 ******************************************************************************
 * @File           : STM32F103C6_RCC_Driver.h
 * @Author         : Abdelrhman Marzoq
 * @Brief          : Driver Header File
 * 			Created on: Jul 22, 2024
 ******************************************************************************
**/

#ifndef _RCC_DRIVER_H
#define _RCC_DRIVER_H

#include "../STM32F103X6.h"

#define HSI_CLK  8000000


							/*******************************************/
							/*           Clock Enable Macros           */
							/*******************************************/

//               GPIOx CLOCK EN
#define RCC_GPIOA_CLK_EN  ((RCC->APB2ENR) |= (1<<2))
#define RCC_GPIOB_CLK_EN  ((RCC->APB2ENR) |= (1<<3))
#define RCC_GPIOC_CLK_EN  ((RCC->APB2ENR) |= (1<<4))
#define RCC_GPIOD_CLK_EN  ((RCC->APB2ENR) |= (1<<5))
#define RCC_GPIOE_CLK_EN  ((RCC->APB2ENR) |= (1<<6))
#define RCC_GPIOF_CLK_EN  ((RCC->APB2ENR) |= (1<<7))
#define RCC_GPIOG_CLK_EN  ((RCC->APB2ENR) |= (1<<8))

//               AFIO CLOCK EN
#define RCC_AFIO_CLK_EN    ((RCC->APB2ENR) |= (1<<0))

//               USART CLOCK EN
#define RCC_USART1_CLK_EN  ((RCC->APB2ENR) |= (1<<14))
#define RCC_USART2_CLK_EN  ((RCC->APB1ENR) |= (1<<17))
#define RCC_USART3_CLK_EN  ((RCC->APB1ENR) |= (1<<18))


uint32_t RCC_GetSysCLKFreq(void);
uint32_t RCC_GetHCLKFreq(void);
uint32_t RCC_GetPCLK1Freq(void);
uint32_t RCC_GetPCLK2Freq(void);


#endif

