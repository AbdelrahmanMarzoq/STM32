/*
 * STM32F103C6_GPIO_Driver.c
 *
 *  Created on: Jul 22, 2024
 *      Author: Abdelrhman Marzoq
 */

#include "STM32F103C6_GPIO_Driver.h"

							/*************************************/
							/*       Function Definition         */
							/*************************************/

uint8_t GetPos(PINy_t PinNumber)
{
	switch(PinNumber)
	{
	case GPIO_PIN0:
		return 0;
		break;
	case GPIO_PIN1:
		return 4;
		break;
	case GPIO_PIN2:
		return 8;
		break;
	case GPIO_PIN3:
		return 12;
		break;
	case GPIO_PIN4:
		return 16;
		break;
	case GPIO_PIN5:
		return 20;
		break;
	case GPIO_PIN6:
		return 24;
		break;
	case GPIO_PIN7:
		return 28;
		break;
	case GPIO_PIN8:
		return 0;
		break;
	case GPIO_PIN9:
		return 4;
		break;
	case GPIO_PIN10:
		return 8;
		break;
	case GPIO_PIN11:
		return 12;
		break;
	case GPIO_PIN12:
		return 16;
		break;
	case GPIO_PIN13:
		return 20;
		break;
	case GPIO_PIN14:
		return 24;
		break;
	case GPIO_PIN15:
		return 28;
		break;
	default :
		return 0;
		break;
	}
}

void GPIO_INIT(GPIO_PinConfig_t *myPin)
{
	volatile uint32_t *configReg = NULL;
	uint8_t Pin_Config = 0x0;
	configReg = (myPin->GPIO_PinNumber < GPIO_PIN8) ? (&(myPin->GPIOx->CRL)) : (&(myPin->GPIOx->CRH));
	// Clear CNF0[1:0] MODE0[1:0]
	(*configReg) &= ~(0xF << GetPos(myPin->GPIO_PinNumber));

	if ( (myPin->GPIO_MODE == GPIO_MODE_OUTPUT_AF_OD) || (myPin->GPIO_MODE == GPIO_MODE_OUTPUT_AF_PP) || (myPin->GPIO_MODE == GPIO_MODE_OUTPUT_OD) || (myPin->GPIO_MODE == GPIO_MODE_OUTPUT_PP))
	{
		// Set CNF0[1:0] -> (his mode)  MODE0[1:0] -> 00
		Pin_Config = ((((myPin->GPIO_MODE - 4) << 2) | (myPin->GPIO_OUTPUT_Speed)) & 0x0F);
	}
	else
	{
		if (myPin->GPIO_MODE == GPIO_MODE_INPUT_AF) // Considered input Floating
		{
			Pin_Config = ((GPIO_MODE_INPUT_FLO << 2 | 0x0) & 0x0F);
		}
		else if (myPin->GPIO_MODE == GPIO_MODE_INPUT_PU)
		{
			Pin_Config = ((GPIO_MODE_INPUT_PU << 2 | 0x0) & 0x0F);
			//From Table 20. Port bit configuration table-> Set ODR->1 to set pin as Input Pull Up
			myPin->GPIOx->ODR |= (myPin->GPIO_PinNumber);
		}
		else if (myPin->GPIO_MODE == GPIO_MODE_INPUT_PD)
		{
			Pin_Config = ((GPIO_MODE_INPUT_PU << 2) & 0x0F);
			//From Table 20. Port bit configuration table-> Set ODR->0 to set pin as Input Pull Down
			myPin->GPIOx->ODR &= ~(myPin->GPIO_PinNumber);
		}
		else
		{
			Pin_Config = ((myPin->GPIO_MODE << 2 | 0x0) & 0x0F);
		}
	}
	// Writing on CRL OR CRH Reg
	(*configReg) |= ((Pin_Config) << GetPos(myPin->GPIO_PinNumber));
}

uint8_t GPIO_READ_PIN(GPIO_PinConfig_t *myPin)
{
	if (myPin->GPIOx->IDR & myPin->GPIO_PinNumber) return 1;
	return 0;
}

uint16_t GPIO_READ_PORT(GPIO_Periphral *GPIOx)
{
	return ((uint16_t)GPIOx->IDR);
}

void GPIO_WRITE_PIN(GPIO_PinConfig_t *myPin, Logic_t Mode)
{
	if (Mode) myPin->GPIOx->ODR |= (myPin->GPIO_PinNumber);
	else      myPin->GPIOx->ODR &= ~(myPin->GPIO_PinNumber);
}

void GPIO_WRITE_PORT(GPIO_Periphral *GPIOx, uint32_t value)
{
	GPIOx->ODR = value;
}

void GPIO_TOGGLE_PIN(GPIO_PinConfig_t *myPin)
{
	myPin->GPIOx->ODR ^= (myPin->GPIO_PinNumber);
}

RETURN_t GPIO_LOCK_PIN(GPIO_PinConfig_t *myPin)
{
	volatile uint32_t temp = 1 << 16;
	temp |= myPin->GPIO_PinNumber;
	// write 1 on bit 16
	myPin->GPIOx->LCKR = temp;
	// write 0 on bit 16
	myPin->GPIOx->LCKR = myPin->GPIO_PinNumber;
	// write 1 on bit 16
	myPin->GPIOx->LCKR = temp;
	// Read 0 from bit 16
	temp = myPin->GPIOx->LCKR;
	// Read 1 From bit 16
	temp = myPin->GPIOx->LCKR;
	if (temp) return OK;
	else return NOK;
}

void GPIO_deINIT(GPIO_Periphral *GPIOx)
{
	if (GPIOx == GPIOA)
	{
		RCC->APB2RSTR |=  (1<<2);
		RCC->APB2RSTR &= ~(1<<2);
	}
	else if (GPIOx == GPIOB)
	{
		RCC->APB2RSTR |=  (1<<3);
		RCC->APB2RSTR &= ~(1<<3);
	}
	else if (GPIOx == GPIOC)
	{
		RCC->APB2RSTR |=  (1<<4);
		RCC->APB2RSTR &= ~(1<<4);
	}
	else if (GPIOx == GPIOD)
	{
		RCC->APB2RSTR |=  (1<<5);
		RCC->APB2RSTR &= ~(1<<5);
	}
	else if (GPIOx == GPIOE)
	{
		RCC->APB2RSTR |=  (1<<6);
		RCC->APB2RSTR &= ~(1<<6);
	}
}
