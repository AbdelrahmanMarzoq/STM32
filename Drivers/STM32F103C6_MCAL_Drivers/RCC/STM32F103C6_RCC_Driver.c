/**
 ******************************************************************************
 * @File           : STM32F103C6_RCC_Driver.c
 * @Author         : Abdelrhman Marzoq
 * @Brief          : RCC Driver
 * 			Created on: Jul 22, 2024
 ******************************************************************************
**/


#include "STM32F103C6_RCC_Driver.h"

static const uint8_t APBPrescTrable[8] = {0, 0, 0, 0, 1, 2, 3, 4};
static const uint8_t AHBPrescTrable[17] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};



//Bits 3:2 SWS[1:0]: System clock switch status
//Set and cleared by hardware to indicate which clock source is used as system clock.
//00: HSI oscillator used as system clock
//01: HSE oscillator used as system clock
//10: PLL used as system clock
//11: Not applicable
uint32_t RCC_GetSysCLKFreq(void)
{
	switch(((RCC->CFGR >> 2) & 0b11))
	{
	case 0: return HSI_CLK;
	/* For future Professional RCC Driver this for External Crystal but default I use Internal Crystal 8MHZ */
	case 1:
	case 2:
	default: return;
	}
}

//Bits of CFGR 7:4 HPRE[3:0]: AHB prescaler
//Set and cleared by software to control AHB clock division factor.
//0xxx: SYSCLK not divided
//1000: SYSCLK divided by 2
//1001: SYSCLK divided by 4
//1010: SYSCLK divided by 8
//1011: SYSCLK divided by 16
//1100: SYSCLK divided by 64
//1101: SYSCLK divided by 128
//1110: SYSCLK divided by 256
//1111: SYSCLK divided by 512
uint32_t RCC_GetHCLKFreq(void)
{
	return (RCC_GetSysCLKFreq() >> (AHBPrescTrable[(RCC->CFGR >> 4) & 0xF]));
}

//Bits of CFGR 10:8 PPRE1[2:0]: APB Low-speed prescaler (APB1)
//Set and cleared by software to control the division factor of the APB Low speed clock (PCLK1).
//0xx: HCLK not divided
//100: HCLK divided by 2
//101: HCLK divided by 4
//110: HCLK divided by 8
//111: HCLK divided by 16
uint32_t RCC_GetPCLK1Freq(void)
{
	return (RCC_GetSysCLKFreq() >> (APBPrescTrable[(RCC->CFGR >> 8) & 0b111]));
}


//Bits of CFGR 13:11 PPRE2[2:0]: APB high-speed prescaler (APB2)
//Set and cleared by software to control the division factor of the APB High speed clock (PCLK2).
//0xx: HCLK not divided
//100: HCLK divided by 2
//101: HCLK divided by 4
//110: HCLK divided by 8
//111: HCLK divided by 16
uint32_t RCC_GetPCLK2Freq(void)
{
	return (RCC_GetSysCLKFreq() >> (APBPrescTrable[(RCC->CFGR >> 11) & 0b111]));
}

