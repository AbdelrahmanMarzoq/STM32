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
#include "../HAL_Drivers/Keypad/Keypad.h"
#include "../HAL_Drivers/7seg/SevenSegment.h"
#include "../STM32F103C6_MCAL_Drivers/EXTI/EXTI.h"



Keypad_t key1;
LCD_4bit_t lcd1;
GPIO_PinConfig_t LED;
_7Segment_t segm;

EXIT_t ext1;


static void myWait(volatile int ms)
{
	for (volatile int i = 0; i < ms; i++)
	{
		for (volatile int j = 0; j < 255; j++);
	}
}

void segm_init()
{
	segm.A.GPIOx = GPIOB;
	segm.A.GPIO_PinNumber = GPIO_PIN9;

	segm.B.GPIOx = GPIOB;
	segm.B.GPIO_PinNumber = GPIO_PIN8;

	segm.C.GPIOx = GPIOB;
	segm.C.GPIO_PinNumber = GPIO_PIN7;

	segm.D.GPIOx = GPIOB;
	segm.D.GPIO_PinNumber = GPIO_PIN15;

	segm.E.GPIOx = GPIOB;
	segm.E.GPIO_PinNumber = GPIO_PIN14;

	segm.F.GPIOx = GPIOB;
	segm.F.GPIO_PinNumber = GPIO_PIN13;

	segm.G.GPIOx = GPIOB;
	segm.G.GPIO_PinNumber = GPIO_PIN12;

}

void GPIO_test()
{
		LED.GPIO_MODE = GPIO_MODE_OUTPUT_OD;
		LED.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
		LED.GPIO_PinNumber = GPIO_PIN1;
		LED.GPIOx = GPIOA;

		GPIO_INIT(&LED);
}


void LCD_init()
{

	lcd1.RS.GPIOx = GPIOB;
	lcd1.RS.GPIO_PinNumber = GPIO_PIN6;

	lcd1.EN.GPIOx = GPIOB;
	lcd1.EN.GPIO_PinNumber = GPIO_PIN5;


	lcd1.Data[0].GPIOx = GPIOA;
	lcd1.Data[0].GPIO_PinNumber = GPIO_PIN11;

	lcd1.Data[1].GPIOx = GPIOA;
	lcd1.Data[1].GPIO_PinNumber = GPIO_PIN10;

	lcd1.Data[2].GPIOx = GPIOA;
	lcd1.Data[2].GPIO_PinNumber = GPIO_PIN9;

	lcd1.Data[3].GPIOx = GPIOA;
	lcd1.Data[3].GPIO_PinNumber = GPIO_PIN8;

}

void KEY_init(){
	key1.ROW[0].GPIO_PinNumber = GPIO_PIN11;
	key1.ROW[0].GPIOx = GPIOB;

	key1.ROW[1].GPIO_PinNumber = GPIO_PIN10;
	key1.ROW[1].GPIOx = GPIOB;

	key1.ROW[2].GPIO_PinNumber = GPIO_PIN1;
	key1.ROW[2].GPIOx = GPIOB;

	key1.ROW[3].GPIO_PinNumber = GPIO_PIN0;
	key1.ROW[3].GPIOx = GPIOB;

	key1.COL[0].GPIO_PinNumber = GPIO_PIN7;
	key1.COL[0].GPIOx = GPIOA;

	key1.COL[1].GPIO_PinNumber = GPIO_PIN6;
	key1.COL[1].GPIOx = GPIOA;

	key1.COL[2].GPIO_PinNumber = GPIO_PIN5;
	key1.COL[2].GPIOx = GPIOA;

	key1.COL[3].GPIO_PinNumber = GPIO_PIN4;
	key1.COL[3].GPIOx = GPIOA;

}
/*
void LCD_init()
{
	lcd2.RS.GPIOx = GPIOB;
	lcd2.RS.GPIO_PinNumber = GPIO_PIN6;

	lcd2.EN.GPIOx = GPIOB;
	lcd2.EN.GPIO_PinNumber = GPIO_PIN5;

	lcd2.Data[0].GPIOx = GPIOB;
	lcd2.Data[0].GPIO_PinNumber = GPIO_PIN4;

	lcd2.Data[1].GPIOx = GPIOB;
	lcd2.Data[1].GPIO_PinNumber = GPIO_PIN3;

	lcd2.Data[2].GPIOx = GPIOA;
	lcd2.Data[2].GPIO_PinNumber = GPIO_PIN15;

	lcd2.Data[3].GPIOx = GPIOA;
	lcd2.Data[3].GPIO_PinNumber = GPIO_PIN12;

	lcd2.Data[4].GPIOx = GPIOA;
	lcd2.Data[4].GPIO_PinNumber = GPIO_PIN11;

	lcd2.Data[5].GPIOx = GPIOA;
	lcd2.Data[5].GPIO_PinNumber = GPIO_PIN10;

	lcd2.Data[6].GPIOx = GPIOA;
	lcd2.Data[6].GPIO_PinNumber = GPIO_PIN9;

	lcd2.Data[7].GPIOx = GPIOA;
	lcd2.Data[7].GPIO_PinNumber = GPIO_PIN8;
}*/

int flag = 0;
void clock_init()
{
	// Enable CLK FOR GPIO B
	RCC_GPIOB_CLK_EN;

	// Enable CLK FOR GPIO A
	RCC_GPIOA_CLK_EN;

	RCC_AFIO_CLK_EN;
}

void Toggle()
{
	flag = 1;
	GPIO_TOGGLE_PIN(&LED);
	LCD_4bit_Set_Cursor(&lcd1, 1, 1);
	LCD_4bit_Print(&lcd1, "Welocme");
}

int main(void)
{
	clock_init();
	LCD_init();
	KEY_init();
	segm_init();
	GPIO_test();

	const uint8_t Char[KEYPAD_ROW_][KEYPAD_COL_] = {{'7', '8', '9', '/'},
													{'4', '5', '6', '*'},
													{'1', '2', '3', '-'},
													{'#', '0', '=', '+'}};
	Key_Init(&key1, Char);
	SevenSegment_init(&segm);

	ext1.EXTI_PIN = EXTI0PA0;
	ext1.Detect_EXTI = RISING;
	ext1.IRQ_EN = EN_EXTI;
	ext1.PF_IRQ = Toggle;
	EXTI_Init(&ext1);
	LCD_4bit_init(&lcd1);

	while(1)
	{
		uint8_t press = Key_get(&key1);
		if (press != ' ')
		{
			LCD_4bit_Print_Char(&lcd1, press);
			SevenSegment_Print(&segm, press);
		}
	}
}





