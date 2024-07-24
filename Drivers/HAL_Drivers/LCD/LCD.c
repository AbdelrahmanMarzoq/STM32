/*
 * LCD.c
 *
 *  Created on: Jul 24, 2024
 *      Author: Abdelrhman Marzoq
 */




#include "LCD.h"


static void myWait(volatile int ms)
{
	for (volatile int i = 0; i < ms; i++)
	{
		for (volatile int j = 0; j < 255; j++);
	}
}

#ifdef _LCD_4BIT_MODE

static void Send_4bit(LCD_4bit_t *LCD, uint8_t _data_command);

							/*************************************/
							/*		 Function Definition		 */
							/*************************************/

void LCD_4bit_init(LCD_4bit_t *LCD)
{
	// init RS O/P
	LCD->RS.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	LCD->RS.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;

	// init EN O/P
	LCD->EN.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	LCD->EN.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;

	// init RS & EN O/P & LOW
	GPIO_INIT(&(LCD->RS));
	GPIO_INIT(&(LCD->EN));

	GPIO_WRITE_PIN(&(LCD->RS), LOW);
	GPIO_WRITE_PIN(&(LCD->EN), LOW);

	// Init data Wires O/P & LOW
	for (char index = 0; index < 4; index++)
	{
		LCD->Data[(int)index].GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		LCD->Data[(int)index].GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
		GPIO_INIT(&(LCD->Data[(int)index]));
		GPIO_WRITE_PIN(&(LCD->Data[(int)index]), LOW);
	}

	// Following init from DataSheet
	// Delay from DataSheet 20 ms in future will write prof delay
	myWait(20);
	// Send this command from DataSheet
	LCD_4bit_Command(LCD, _LCD_4BIT_MODE | _LCD_2LINE);
	// Delay from DataSheet 5 ms in future will write prof delay
	myWait(5);
	LCD_4bit_Command(LCD, _LCD_4BIT_MODE | _LCD_2LINE);
	// Delay from DataSheet 100 micro sec in future will write prof delay
	myWait(1);
	LCD_4bit_Command(LCD, _LCD_4BIT_MODE | _LCD_2LINE);

	LCD_4bit_Command(LCD, _LCD_CLEAR);

	LCD_4bit_Command(LCD, _LCD_RETURN_HOME);

	LCD_4bit_Command(LCD, _LCD_DISPLAY_ON | _LCD_CURSOR_ON);

	LCD_4bit_Command(LCD, _LCD_ENTRY_MODE);

	LCD_4bit_Command(LCD, _LCD_DDRAM_START);


}

void LCD_4bit_Set_Cursor(LCD_4bit_t *LCD, uint8_t row, uint8_t column){
	switch(row)
	{
	case 1:
		LCD_4bit_Command(LCD, _LCD_ROW1_START + column);break;
	case 2:
		LCD_4bit_Command(LCD, _LCD_ROW2_START + column);break;
	}
}

void LCD_4bit_Command(LCD_4bit_t *LCD, uint8_t command){

	// To Tell LCD That i will send command Write RS LOW @ref INFO (LCD.H)
	GPIO_WRITE_PIN(&(LCD->RS), LOW);
	myWait(300);
	// Send High Nipple on Command Wire
	Send_4bit(LCD, command >> 4);
	// Send Low Nipple on Command Wire
	Send_4bit(LCD, command);
}

void LCD_4bit_Print_Char(LCD_4bit_t *LCD, uint8_t Data){

	// To Tell LCD That i will send Data Write RS HIGH @ref INFO (LCD.H)
	GPIO_WRITE_PIN(&(LCD->RS), HIGH);
	myWait(300);
	// Send High Nibble on Data Wire
	Send_4bit(LCD, Data>>4);

	// Send Low Nibble on Data Wire
	Send_4bit(LCD, Data);
}

void LCD_4bit_Print_String(LCD_4bit_t *LCD, uint8_t *data){
	static char Counter = 0;
	while(*data)
	{
		if ( (Counter < 32) && (Counter != 16))
		{
			LCD_4bit_Print_Char(LCD, *data++);
			Counter++;
		}
		else if (Counter == 16)
		{
			LCD_4bit_Set_Cursor(LCD, 2, 0);
			LCD_4bit_Print_Char(LCD, *data++);
			Counter++;
		}
		else
		{
			LCD_4bit_Set_Cursor(LCD, 1, 0);
			LCD_4bit_Print_Char(LCD, *data++);
			Counter = 1;
		}
	}
}

void LCD_4bit_Print_Custom_char(LCD_4bit_t *LCD, const uint8_t c_char[]){

}

static void Send_4bit(LCD_4bit_t *LCD, uint8_t _data_command)
{
	// Send 4bits data | command bit by bit
	for (char i = 0; i < 4; i++)
	{
		GPIO_WRITE_PIN( &(LCD->Data[(int)i]), ((_data_command >> (int)i) & 0x01) );
	}
	myWait(500);
	// Enable Signal to Reccieve Data or Command
	GPIO_WRITE_PIN(&(LCD->EN), HIGH);
	myWait(30);
	GPIO_WRITE_PIN(&(LCD->EN), LOW);
	myWait(30);
}

#endif


#ifdef _LCD_8BIT_MODE

static void Send_8bit(LCD_8bit_t *LCD, uint8_t _data_command);


						/*************************************/
						/*		 Function Definition		 */
						/*************************************/


void LCD_8bit_init(LCD_8bit_t *LCD)
{
	LCD->RS.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		LCD->RS.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;

		LCD->EN.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		LCD->EN.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;

		GPIO_INIT(&(LCD->RS));
		GPIO_INIT(&(LCD->EN));

		GPIO_WRITE_PIN(&(LCD->RS), LOW);
		GPIO_WRITE_PIN(&(LCD->EN), LOW);

		for (char index = 0; index < 8; index++)
		{
			LCD->Data[index].GPIO_MODE = GPIO_MODE_OUTPUT_PP;
			LCD->Data[index].GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
			GPIO_INIT(&(LCD->Data[index]));
			GPIO_WRITE_PIN(&(LCD->Data[index]), LOW);
		}
		for (volatile char delay = 0; delay < 100; delay++);
		LCD_8bit_Command(LCD, _LCD_8BIT_MODE | _LCD_2LINE);
}

void LCD_8bit_Set_Cursor(LCD_8bit_t *LCD, uint8_t row, uint8_t column)
{
	switch(row)
		{
		case 1:
			LCD_8bit_Command(LCD, _LCD_ROW1_START + column);break;
		case 2:
			LCD_8bit_Command(LCD, _LCD_ROW2_START + column);break;
		}
}

void LCD_8bit_Command(LCD_84bit_t *LCD, uint8_t command)
{
	// To Tell LCD That i will send command Write RS LOW @ref INFO (LCD.H)
	GPIO_WRITE_PIN(&(LCD->RS), LOW);
	// Send 8bits Command on Wires Bit by Bit
	Send_8bit(LCD, command);
}

void LCD_8bit_Print_Char(LCD_8bit_t *LCD, uint8_t data){
	// To Tell LCD That i will send Data Write RS HIGH @ref INFO (LCD.H)
	GPIO_WRITE_PIN(&(LCD->RS), HIGH);

	// Send 8bits data on Wires Bit by Bit
	Send_8bit(LCD, data);
}

void LCD_8bit_Print_String(LCD_8bit_t *LCD, uint8_t *data){
	static char Counter = 0;
		while(*data)
		{
			if ( (Counter < 32) && (Counter != 16))
			{
				LCD_8bit_Print_Char(LCD, *data++);
				Counter++;
			}
			else if (Counter == 16)
			{
				LCD_8bit_Set_Cursor(LCD, 2, 0);
				LCD_8bit_Print_Char(LCD, *data++);
				Counter++;
			}
			else
			{
				LCD_8bit_Command(LCD, 1, 0);
				LCD_8bit_Print_Char(LCD, *data++);
				Counter = 1;
			}
		}
}

void LCD_8bit_Print_Custom_char(LCD_8bit_t *LCD, const uint8_t c_char[]){

}

static void Send_8bit(LCD_8bit_t *LCD, uint8_t _data_command)
{
	// Send 4bits data | command bit by bit
	for (char i = 0; i < 8; i++)
	{
		GPIO_WRITE_PIN( &(LCD->Data[(int)i]), ((_data_command >> (int)i) & 0x01) );
	}

	// Enable Signal to Reccieve Data or Command
	GPIO_WRITE_PIN(&(LCD->EN), HIGH);
	for(volatile char delay = 0; delay < 250; delay++);  //Small delay i will write professional delay after learning it
	GPIO_WRITE_PIN(&(LCD->EN), LOW);
	for(volatile char delay = 0; delay < 250; delay++);
}

#endif



void Convert_Byte_To_String(const uint8_t value, uint8_t *str)
{

}

void Convert_Short_To_String(const uint16_t value, uint8_t *str)
{

}

void Convert_int_To_String(const uint32_t value, uint8_t *str)
{

}






