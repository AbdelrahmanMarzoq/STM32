/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Abdelrhman Marzoq
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

#include "../HAL_Drivers/LCD/LCD.h"



LCD_4bit_t lcd1;



static void myWait(volatile int ms)
{
	for (volatile int i = 0; i < ms; i++)
	{
		for (volatile int j = 0; j < 255; j++);
	}
}

void LCD_init()
{


	lcd1.RS.GPIOx = GPIOB;
	lcd1.RS.GPIO_PinNumber = GPIO_PIN6;

	lcd1.EN.GPIOx = GPIOB;
	lcd1.EN.GPIO_PinNumber = GPIO_PIN5;


	lcd1.Data[0].GPIOx = GPIOA;
	lcd1.Data[0].GPIO_PinNumber = GPIO_PIN15;


	lcd1.Data[1].GPIOx = GPIOA;
	lcd1.Data[1].GPIO_PinNumber = GPIO_PIN12;


	lcd1.Data[2].GPIOx = GPIOA;
	lcd1.Data[2].GPIO_PinNumber = GPIO_PIN11;


	lcd1.Data[3].GPIOx = GPIOA;
	lcd1.Data[3].GPIO_PinNumber = GPIO_PIN10;

}

void clock_init()
{
	// Enable CLK FOR GPIO B
	RCC_GPIOB_CLK_EN;

	// Enable CLK FOR GPIO A
	RCC_GPIOA_CLK_EN;
}


int main(void)
{
	clock_init();

	LCD_init();
	LCD_4bit_init(&lcd1);

	while(1)
	{
			LCD_4bit_Print_Char(&lcd1, 'A');
			myWait(1000);
	}
}



