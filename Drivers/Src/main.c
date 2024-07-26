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


Keypad_t key1;

LCD_8bit_t lcd2;

GPIO_PinConfig_t LED;


static void myWait(volatile int ms)
{
	for (volatile int i = 0; i < ms; i++)
	{
		for (volatile int j = 0; j < 255; j++);
	}
}

void GPIO_test()
{
		LED.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		LED.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
		LED.GPIO_PinNumber = GPIO_PIN0;
		LED.GPIOx = GPIOA;

		GPIO_INIT(&LED);
		GPIO_WRITE_PIN(&LED, HIGH);
		myWait(20);
		GPIO_WRITE_PIN(&LED, HIGH);

}

/*
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

}*/

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
	KEY_init();
	const uint8_t Char[KEYPAD_ROW_][KEYPAD_COL_] = {{'7', '8', '9', '/'},
													{'4', '5', '6', '*'},
													{'1', '2', '3', '-'},
													{'#', '0', '=', '+'}};
	Key_Init(&key1, Char);
	LCD_8bit_init(&lcd2);
	GPIO_test();

	uint8_t Character[8] ={ 0b00000, 0b00000, 0b01010, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000 };
	LCD_8bit_Print(&lcd2, "Hello World!");

//	LCD_8bit_Set_Cursor(&lcd2, 2, 10);
//	LCD_8bit_Print_Custom_char(&lcd2, Character, 2);

//	myWait(1000);
//	LCD_8bit_Display_OFF(&lcd2);
//	myWait(1000);
//	LCD_8bit_Display_ON(&lcd2);
	LCD_8bit_Set_Cursor(&lcd2, 2, 10);

	while(1)
	{

		uint8_t press = Key_get(&key1);
		if(press)   LCD_8bit_Print_Char(&lcd2, press);

		GPIO_TOGGLE_PIN(&LED);
		myWait(10);

	}
}



