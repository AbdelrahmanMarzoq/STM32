/**
 ******************************************************************************
 * @File           : STM32F103X6.h
 * @Author         : Abdelrhman Marzoq
 * @Brief          : Header for STM32
 * 			Created on: Jul 22, 2024
 ******************************************************************************
**/

#ifndef STM32F103X6_H_
#define STM32F103X6_H_


							/*******************************************/
							/*            Include Section              */
							/*******************************************/
#include "stdlib.h"
#include "stdio.h"
#include <stdint.h>

							/*******************************************/
							/*      Base addresses for Memories        */
							/*******************************************/

#define FLASH_MEMORY_BASE				 0x08000000UL
#define SYSTEM_MEMORY_BASE 				 0x1FFFF000UL
#define SRAM_MEMORY_BASE       			 0x20000000UL

#define PERIPHRALS_BASE         		 0x20000000UL
#define INTERNAL_PERIPHRALS_BASE         0xE0000000UL   // Internal CortexM3




							/*******************************************/
							/*     Base addresses for AHB Periphrals   */
							/*******************************************/

#define RCC_BASE				 		 (0x40021000UL)

							/*******************************************/
							/*    Base addresses for APB2 Periphrals   */
							/*******************************************/


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

//            USART1
#define USART1_BASE						 (0x40013800UL)

//            I2C1
#define I2C1_BASE						 (0x40005400UL)

//            I2C2
#define I2C2_BASE						 (0x40005800UL)


//            SPI1
#define SPI1_BASE						 (0x40013000UL)


							/*******************************************/
							/*    Base addresses for APB1 Periphrals   */
							/*******************************************/

//            USART2
#define USART2_BASE						 (0x40004400UL)

//            USART3
#define USART3_BASE						 (0x40004800UL)

//            SPI2
#define SPI2_BASE						 (0x40003800UL)

							/*******************************************/
							/*         Peripheral register : RCC       */
							/*******************************************/
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


							/*******************************************/
							/*         Peripheral register : GPIO       */
							/*******************************************/
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



							/*******************************************/
							/*         Peripheral register : AFIO     */
							/*******************************************/
typedef struct
{
	volatile uint32_t EVCR;
	volatile uint32_t MAPR;
	volatile uint32_t EXTICR[4];
	volatile uint32_t RESERVED;
	volatile uint32_t MAPR2;
}AFIO_Periphral;


							/*******************************************/
							/*         Peripheral register : EXTI      */
							/*******************************************/
typedef struct
{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_Periphral;

							/*******************************************/
							/*         Peripheral register : USART     */
							/*******************************************/
typedef struct
{
	volatile uint32_t  SR;
	volatile uint32_t  DR;
	volatile uint32_t  BRR;
	volatile uint32_t  CR1;
	volatile uint32_t  CR2;
	volatile uint32_t  CR3;
	volatile uint32_t  GTPR;
}USART_Periphral;


							/*******************************************/
							/*         Peripheral register : I2C       */
							/*******************************************/
typedef struct
{
	volatile uint32_t  CR1;
	volatile uint32_t  CR2;
	volatile uint32_t  OAR1;
	volatile uint32_t  OAR2;
	volatile uint32_t  DR;
	volatile uint32_t  SR1;
	volatile uint32_t  SR2;
	volatile uint32_t  CCR;
	volatile uint32_t  TRISE;
}I2C_Periphral;

							/*******************************************/
							/*         Peripheral register : SPI       */
							/*******************************************/

typedef struct
{
	volatile uint32_t  CR1;
	volatile uint32_t  CR2;
	volatile uint32_t  SR;
	volatile uint32_t  DR;
	volatile uint32_t  CRCPR;
	volatile uint32_t  RXCRCR;
	volatile uint32_t  TXCRCR;
	volatile uint32_t  I2SCFGR;
	volatile uint32_t  I2SPR;
}SPI_Periphral;

							/*******************************************/
							/*         Peripheral Instants: GPIO       */
							/*******************************************/

#define GPIOA				((GPIO_Periphral *)GPIOA_BASE)
#define GPIOB				((GPIO_Periphral *)GPIOB_BASE)
#define GPIOC				((GPIO_Periphral *)GPIOC_BASE)
#define GPIOD				((GPIO_Periphral *)GPIOD_BASE)
#define GPIOE				((GPIO_Periphral *)GPIOE_BASE)
#define GPIOF				((GPIO_Periphral *)GPIOF_BASE)
#define GPIOG				((GPIO_Periphral *)GPIOG_BASE)


							/*******************************************/
							/*         Peripheral Instants: EXTI      */
							/*******************************************/
#define EXTI				    ((EXTI_Periphral *)EXTI_BASE)

							/*******************************************/
							/*         Peripheral Instants: RCC        */
							/*******************************************/

#define RCC					    ((RCC_Periphral *)RCC_BASE)

							/*******************************************/
							/*         Peripheral Instants: AFIO       */
							/*******************************************/
#define AFIO					((AFIO_Periphral *)AFIO_BASE)

							/*******************************************/
							/*         Peripheral Instants: USART      */
							/*******************************************/
#define USART1					((USART_Periphral *)USART1_BASE)
#define USART2					((USART_Periphral *)USART2_BASE)
#define USART3					((USART_Periphral *)USART3_BASE)



							/*******************************************/
							/*         Peripheral Instants: I2C        */
							/*******************************************/
#define I2C1					((I2C_Periphral *)I2C1_BASE)
#define I2C2					((I2C_Periphral *)I2C2_BASE)

							/*******************************************/
							/*         Peripheral Instants: SPI        */
							/*******************************************/
#define SPI1					((SPI_Periphral *)SPI1_BASE)
#define SPI2					((SPI_Periphral *)SPI2_BASE)



							/*******************************************/
							/*           IRQ Enable & Numbers          */
							/*******************************************/

// Hardware Position in NVIC At Cortex M3 STM32
#define EXTI0_IRQ			6
#define EXTI1_IRQ			7
#define EXTI2_IRQ			8
#define EXTI3_IRQ			9
#define EXTI4_IRQ			10

// From EXTI5 To EXTI9 have the same number on NVIC so These Have Same Interrupt Handler
#define EXTI5_9_IRQ			23
#define EXTI5_IRQ			EXTI5_9_IRQ
#define EXTI6_IRQ			EXTI5_9_IRQ
#define EXTI7_IRQ			EXTI5_9_IRQ
#define EXTI8_IRQ			EXTI5_9_IRQ
#define EXTI9_IRQ			EXTI5_9_IRQ

// From EXTI10 To EXTI15 have the same number on NVIC so These Have Same Interrupt Handler
#define EXTI10_15_IRQ		40
#define EXTI10_IRQ			EXTI10_15_IRQ
#define EXTI11_IRQ			EXTI10_15_IRQ
#define EXTI12_IRQ			EXTI10_15_IRQ
#define EXTI13_IRQ			EXTI10_15_IRQ
#define EXTI14_IRQ			EXTI10_15_IRQ
#define EXTI15_IRQ			EXTI10_15_IRQ


// USART IRQ Number
#define USART1_IRQ			37
#define USART2_IRQ			38
#define USART3_IRQ			39


// I2C IRQ Number
#define I2C1_EV_IRQ			31
#define I2C1_ER_IRQ			32

#define I2C2_EV_IRQ			33
#define I2C2_ER_IRQ			34

// SPI IRQ Number
#define  SPI1_IRQ			35
#define  SPI2_IRQ			36


						/*******************************************/
						/*     Base addresses for NVIC Periphral   */
						/*******************************************/
#define NVIC_BASE				   		 0xE000E100UL

						/*******************************************/
						/*         Peripheral register : NVIC      */
						/*******************************************/
#define NVIC_ISER0						 (*(volatile uint32_t *)(NVIC_BASE + 0x00))    // Enable IRQ  0  : 31
#define NVIC_ISER1						 (*(volatile uint32_t *)(NVIC_BASE + 0x04))	   // Enable IRQ  32 : 63
#define NVIC_ISER2						 (*(volatile uint32_t *)(NVIC_BASE + 0x08))	   // Enable IRQ  64 : 67

#define NVIC_ICER0						 (*(volatile uint32_t *)(NVIC_BASE + 0x80))	   // Disable IRQ 0  : 31
#define NVIC_ICER1						 (*(volatile uint32_t *)(NVIC_BASE + 0x84))	   // Disable IRQ 32 : 63
#define NVIC_ICER2						 (*(volatile uint32_t *)(NVIC_BASE + 0x88))    // Disable IRQ 64 : 67



						/*******************************************/
						/*  Enable | Disable NVIC Mask at CortexM3 */
						/*******************************************/
/**********************************  Enable  *************************************/
//                               Enable EXTI_IRQ
#define NVIC_Enable_EXTI0				 (NVIC_ISER0 |= 1 << EXTI0_IRQ)
#define NVIC_Enable_EXTI1				 (NVIC_ISER0 |= 1 << EXTI1_IRQ)
#define NVIC_Enable_EXTI2				 (NVIC_ISER0 |= 1 << EXTI2_IRQ)
#define NVIC_Enable_EXTI3				 (NVIC_ISER0 |= 1 << EXTI3_IRQ)
#define NVIC_Enable_EXTI4				 (NVIC_ISER0 |= 1 << EXTI4_IRQ)
#define NVIC_Enable_EXTI5_9				 (NVIC_ISER0 |= 1 << EXTI5_9_IRQ)
#define NVIC_Enable_EXTI10_15			 (NVIC_ISER1 |= 1 << (EXTI10_15_IRQ - 32))

//                               Enable USART_IRQ
#define NVIC_Enable_USART1               (NVIC_ISER1 |= 1 << (USART1_IRQ - 32))
#define NVIC_Enable_USART2               (NVIC_ISER1 |= 1 << (USART2_IRQ - 32))
#define NVIC_Enable_USART3               (NVIC_ISER1 |= 1 << (USART3_IRQ - 32))

//                               Enable I2C_IRQ
#define NVIC_Enable_EV_I2C1              (NVIC_ISER0 |= 1 <<  I2C1_EV_IRQ	   )
#define NVIC_Enable_ER_I2C1              (NVIC_ISER1 |= 1 << (I2C1_ER_IRQ - 32))
#define NVIC_Enable_EV_I2C2              (NVIC_ISER1 |= 1 << (I2C2_EV_IRQ - 32))
#define NVIC_Enable_ER_I2C2              (NVIC_ISER1 |= 1 << (I2C2_ER_IRQ - 32))


//                               Enable SPI_IRQ
#define NVIC_Enable_SPI1              (NVIC_ISER1 |= 1 << (SPI1_IRQ - 32))
#define NVIC_Enable_SPI2              (NVIC_ISER1 |= 1 << (SPI2_IRQ - 32))


/**********************************  Disable  *************************************/
//                               Disable EXTI_IRQ
#define NVIC_Disable_EXTI0				 (NVIC_ICER0 |= 1 << EXTI0_IRQ)
#define NVIC_Disable_EXTI1				 (NVIC_ICER0 |= 1 << EXTI1_IRQ)
#define NVIC_Disable_EXTI2				 (NVIC_ICER0 |= 1 << EXTI2_IRQ)
#define NVIC_Disable_EXTI3				 (NVIC_ICER0 |= 1 << EXTI3_IRQ)
#define NVIC_Disable_EXTI4				 (NVIC_ICER0 |= 1 << EXTI4_IRQ)
#define NVIC_Disable_EXTI5_9			 (NVIC_ICER0 |= 1 << EXTI5_9_IRQ)
#define NVIC_Disable_EXTI10_15			 (NVIC_ICER1 |= 1 << (EXTI10_15_IRQ - 32))

//                               Disable USART_IRQ
#define NVIC_Disable_USART1              (NVIC_ICER1 |= 1 << (USART1_IRQ - 32))
#define NVIC_Disable_USART2              (NVIC_ICER1 |= 1 << (USART2_IRQ - 32))
#define NVIC_Disable_USART3              (NVIC_ICER1 |= 1 << (USART3_IRQ - 32))


//                               Disable I2C_IRQ
#define NVIC_Disable_EV_I2C1             (NVIC_ISER0 |= 1 <<  I2C1_EV_IRQ      )
#define NVIC_Disable_ER_I2C1             (NVIC_ISER1 |= 1 << (I2C1_ER_IRQ - 32))
#define NVIC_Disable_EV_I2C2             (NVIC_ISER1 |= 1 << (I2C2_EV_IRQ - 32))
#define NVIC_Disable_ER_I2C2             (NVIC_ISER1 |= 1 << (I2C2_ER_IRQ - 32))

//                               Disable SPI_IRQ
#define NVIC_Disable_SPI1          	 	 (NVIC_ISER1 |= 1 << (SPI1_IRQ - 32))
#define NVIC_Disable_SPI2           	 (NVIC_ISER1 |= 1 << (SPI2_IRQ - 32))




#endif /* STM32F103X6_H_ */
