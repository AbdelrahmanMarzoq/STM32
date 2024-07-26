/*
 * STM32F103X6.h
 *
 *  Created on: Jul 22, 2024
 *      Author: Abdelrhman Marzoq
 */

#ifndef STM32F103X6_H_
#define STM32F103X6_H_


//------------------------------------
//             Includes
//------------------------------------
#include "stdlib.h"
#include "stdio.h"
#include <stdint.h>



//------------------------------------
//  Base addresses for Memories
//------------------------------------

#define FLASH_MEMORY_BASE				 0x08000000UL
#define SYSTEM_MEMORY_BASE 				 0x1FFFF000UL
#define SRAM_MEMORY_BASE       			 0x20000000UL

#define PERIPHRALS_BASE         		 0x20000000UL
#define INTERNAL_PERIPHRALS_BASE         0xE0000000UL

//------------------------------------
//  Base addresses for AHB Periphrals
//------------------------------------

#define RCC_BASE				 		 (0x40021000UL)


//------------------------------------
//  Base addresses for APB2 Periphrals
//------------------------------------

//            GPIO PORTS
// GPIO A&B Fully included in LQFP48
#define GPIOA_BASE 						 (0x40010800UL)
#define GPIOB_BASE 						 (0x40010C00UL)
// GPIO C&D Partially inlcuded in LQFP48
#define GPIOC_BASE 						 (0x40011000UL)
#define GPIOD_BASE 						 (0x40011400UL)
// GPIO E&F&G Not inlcuded in LQFP48
#define GPIOE_BASE 						 (0x40011800UL)
#define GPIOF_BASE 						 (0x40011C00UL)
#define GPIOG_BASE 						 (0x40012000UL)

//            EXTI
#define EXTI_BASE						 (0x40010400UL)

//            AFIO
#define AFIO_BASE 						 (0x40010000UL)

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//       Peripheral register : RCC
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
typedef struct
{
	volatile uint32_t CR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t APB2RSTR;
	volatile uint32_t APB1RSTR;
	volatile uint32_t AHBENR;
	volatile uint32_t APB2ENR;
	volatile uint32_t APB1ENR;
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
}RCC_Periphral;



//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//       Peripheral register : GPIO
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
typedef struct
{
	volatile uint32_t CRL;
	volatile uint32_t CRH;
	volatile uint32_t IDR;
	volatile uint32_t ODR;
	volatile uint32_t BSRR;
	volatile uint32_t BRR;
	volatile uint32_t LCKR;

}GPIO_Periphral;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//       Peripheral register : AFIO
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
typedef struct
{
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR1;
	volatile uint32_t EXTICR2;
	volatile uint32_t EXTICR3;
	volatile uint32_t EXTICR4;
	volatile uint32_t RESERVED;
	volatile uint32_t MAPR2;
}AFIO_Periphral;

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//       Peripheral register : EXTI
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_Periphral;





//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//       Peripheral Instants: GPIO
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

#define GPIOA				((GPIO_Periphral *)GPIOA_BASE)
#define GPIOB				((GPIO_Periphral *)GPIOB_BASE)
#define GPIOC				((GPIO_Periphral *)GPIOC_BASE)
#define GPIOD				((GPIO_Periphral *)GPIOD_BASE)
#define GPIOE				((GPIO_Periphral *)GPIOE_BASE)
#define GPIOF				((GPIO_Periphral *)GPIOF_BASE)
#define GPIOG				((GPIO_Periphral *)GPIOG_BASE)


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//       Peripheral Instants: EXTI
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
#define EXTI				    ((EXTI_Periphral *)EXTI_BASE)

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//       Peripheral Instants: RCC
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
#define RCC					    ((RCC_Periphral *)RCC_BASE)

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//       Peripheral Instants: AFIO
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
#define AFIO					((AFIO_Periphral *)RCC_BASE)

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//       clock enable Macros
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

//               GPIO CLOCK EN
#define RCC_GPIOA_CLK_EN  ((RCC->APB2ENR) |= (1<<2))
#define RCC_GPIOB_CLK_EN  ((RCC->APB2ENR) |= (1<<3))
#define RCC_GPIOC_CLK_EN  ((RCC->APB2ENR) |= (1<<4))
#define RCC_GPIOD_CLK_EN  ((RCC->APB2ENR) |= (1<<5))
#define RCC_GPIOE_CLK_EN  ((RCC->APB2ENR) |= (1<<6))
#define RCC_GPIOF_CLK_EN  ((RCC->APB2ENR) |= (1<<7))
#define RCC_GPIOG_CLK_EN  ((RCC->APB2ENR) |= (1<<8))

//               AFIO CLOCK EN
#define RCC_AFIO_CLK_EN ((RCC->APB2ENR) |= (1<<0));




//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
//        Generic Macros:
//-*-*-*-*-*-*-*-*-*-*--*-*-*-*-*-*-*



#endif /* STM32F103X6_H_ */
