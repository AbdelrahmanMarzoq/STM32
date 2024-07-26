/*
 * LCD.c
 *
 *  Created on: Jul 24, 2024
 *      Author: Abdelrhman Marzoq
 */


							/*************************************/
							/*	  	     Include Section         */
							/*************************************/
#include "LCD.h"


							/*************************************/
							/*	   Static Function Declaration   */
							/*************************************/


static void myWait(volatile int ms);

#ifdef _LCD_4BIT_MODE

#define _8BIT_MODE		0x30

							/*************************************/
							/*	   Static Function Declaration   */
							/*************************************/

static void Send_4bit(LCD_4bit_t *LCD, uint8_t _data_command);
static void Send_CGRAM(LCD_4bit_t *LCD, uint8_t data);

							/*************************************/
							/*		 Function Definition		 */
							/*************************************/


static char AddressDD = 0;

void LCD_4bit_init(LCD_4bit_t *LCD)
{
	// Init RS & EN & DATA Lines
		LCD->RS.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		LCD->RS.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
		LCD->EN.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		LCD->EN.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
		GPIO_INIT(&(LCD->RS));
		GPIO_INIT(&(LCD->EN));
		GPIO_WRITE_PIN(&(LCD->RS), LOW);
		GPIO_WRITE_PIN(&(LCD->EN), LOW);
		for (char index = 0; index < 4; index++)
		{
			LCD->Data[(int)index].GPIO_MODE = GPIO_MODE_OUTPUT_PP;
			LCD->Data[(int)index].GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
			GPIO_INIT(&(LCD->Data[(int)index]));
			GPIO_WRITE_PIN(&(LCD->Data[(int)index]), LOW);
		}

		// Following init from DataSheet
		// Delay from DataSheet 20 ms in future will write prof delay
		myWait(2000);
		// Send this command from DataSheet
		LCD_4bit_Command(LCD, _8BIT_MODE | _LCD_2LINE);
		// Delay from DataSheet 5 ms in future will write prof delay
		myWait(2000);
		LCD_4bit_Command(LCD, _8BIT_MODE | _LCD_2LINE);
		// Delay from DataSheet 100 micro sec in future will write prof delay
		myWait(2000);
		// Init it with 8bit mode and 2 Line 5*8
		LCD_4bit_Command(LCD, _8BIT_MODE | _LCD_2LINE | _LCD_DOT_MATRIX_7);
		myWait(2000);
		LCD_4bit_Command(LCD, _LCD_4BIT_MODE | _LCD_2LINE | _LCD_DOT_MATRIX_7);

		// Send command to clear Screen
		LCD_4bit_Command(LCD, _LCD_CLEAR);
		// Send command to set cursor at row 1 column 1
		LCD_4bit_Command(LCD, _LCD_RETURN_HOME);
		// Send command to Display on and Blink cursor on
		LCD_4bit_Command(LCD, _LCD_DISPLAY_ON | _LCD_CURSOR_ON);
		// Send command to set cursor increamenet toward right after writing without shift display
		LCD_4bit_Command(LCD, _LCD_INC_CURSOR_SHIFT_OFF);
		// Send command that i ready to recieve data so i make the AC (Address counter) to First address in DDRAM
		LCD_4bit_Command(LCD, _LCD_DDRAM_START);

}

void LCD_4bit_Set_Cursor(LCD_4bit_t *LCD, uint8_t row, uint8_t column){
	if ( (row == 2 || row == 1) && ((column > 0) && (column <= 16)) )
	{
		switch(row)
		{
		case 1:
			// Send command to Make AC (Address counter) point to The target column in row 1
			LCD_4bit_Command(LCD, _LCD_ROW1_START + --column);
			AddressDD = 0 + column;
			break;
		case 2:
			// Send command to Make AC (Address counter) point to The target column in row 2
			LCD_4bit_Command(LCD, _LCD_ROW2_START + --column);
			AddressDD = 16 + column;
			break;
		}
	}
}

void LCD_4bit_Command(LCD_4bit_t *LCD, uint8_t command){

	// To Tell LCD That i will send command Write RS LOW @ref INFO (LCD.H)
	GPIO_WRITE_PIN(&(LCD->RS), LOW);
	// Send High Nibble on Command Wire
	Send_4bit(LCD, (command>>4) & 0x0F);
	// Send Low Nibble on Command Wire
	Send_4bit(LCD, command);
}

static void LCD_4bit_Print_Char(LCD_4bit_t *LCD, uint8_t Data){

	if (AddressDD == 16) LCD_4bit_Set_Cursor(LCD, 2, 1);
	else if (AddressDD == 32)
	{
		LCD_4bit_Set_Cursor(LCD, 1, 1);
		AddressDD = 0;
	}
	AddressDD++;
	// To Tell LCD That i will send Data Write RS HIGH @ref INFO (LCD.H)
	GPIO_WRITE_PIN(&(LCD->RS), HIGH);
	// Send High Nibble on Data Wire
	Send_4bit(LCD, (Data>>4) & 0x0F);
	// Send Low Nibble on Data Wire
	Send_4bit(LCD, Data);
}

void LCD_4bit_Print(LCD_4bit_t *LCD, uint8_t *data){
	while(*data) LCD_4bit_Print_Char(LCD, *data++);
}

void LCD_4bit_Print_Custom_char(LCD_4bit_t *LCD, const uint8_t c_char[], uint8_t Pos){
	Pos &= 0x07;
	LCD_4bit_Command(LCD, _LCD_CGRAM_START + (Pos*8));
	for(int i = 0; i < 8; i ++)
	{
		Send_CGRAM(LCD, c_char[i]);
	}
	LCD_4bit_Command(LCD, _LCD_DDRAM_START);
	if (AddressDD < 16) LCD_4bit_Command(LCD, _LCD_ROW1_START + AddressDD);
	else LCD_4bit_Command(LCD,_LCD_ROW2_START + (AddressDD - 16));
	LCD_4bit_Print_Char(LCD, Pos);
}

static void Send_CGRAM(LCD_4bit_t *LCD, uint8_t data){
	// To Tell LCD That i will send Data Write RS HIGH @ref INFO (LCD.H)
	GPIO_WRITE_PIN(&(LCD->RS), HIGH);
	// Send 8bits data on Wires Bit by Bit
	Send_4bit(LCD, data);
}

static void Send_4bit(LCD_4bit_t *LCD, uint8_t _data_command)
{
	// Send 4bits data | command bit by bit
	for (char i = 0; i < 4; i++)
	{
		GPIO_WRITE_PIN( &(LCD->Data[(int)i]), ((_data_command >> (int)i) & 0x01) );
	}
	// Enable Signal to Reccieve Data or Command
	GPIO_WRITE_PIN(&(LCD->EN), HIGH);
	myWait(1);
	GPIO_WRITE_PIN(&(LCD->EN), LOW);
	myWait(2);
}

void LCD_4bit_Clear(LCD_4bit_t *LCD)
{
	// To Clear LCD & DDRAM Memory
	LCD_4bit_Command(LCD, _LCD_CLEAR);
}

#endif


#ifdef _LCD_8BIT_MODE

						/*************************************/
						/*	   Static Function Declaration   */
						/*************************************/

static void Send_8bit(LCD_8bit_t *LCD, uint8_t _data_command);
static void Send_CGRAM(LCD_8bit_t *LCD, uint8_t data);

static char AddressDD = 0;

						/*************************************/
						/*		 Function Definition		 */
						/*************************************/


void LCD_8bit_init(LCD_8bit_t *LCD)
{
	// Init RS & EN & DATA Lines
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
		LCD->Data[(int)index].GPIO_MODE = GPIO_MODE_OUTPUT_PP;
		LCD->Data[(int)index].GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
		GPIO_INIT(&(LCD->Data[(int)index]));
		GPIO_WRITE_PIN(&(LCD->Data[(int)index]), LOW);
	}

	// Following init from DataSheet
	// Delay from DataSheet 20 ms in future will write prof delay
	myWait(20);
	// Send this command from DataSheet
	LCD_8bit_Command(LCD, _LCD_8BIT_MODE | _LCD_2LINE);
	// Delay from DataSheet 5 ms in future will write prof delay
	myWait(5);
	LCD_8bit_Command(LCD, _LCD_8BIT_MODE | _LCD_2LINE);
	// Delay from DataSheet 100 micro sec in future will write prof delay
	myWait(1);
	// Init it with 8bit mode and 2 Line 5*8
	LCD_8bit_Command(LCD, _LCD_8BIT_MODE | _LCD_2LINE | _LCD_DOT_MATRIX_7);

	// Send command to clear Screen
	LCD_8bit_Command(LCD, _LCD_CLEAR);
	// Send command to set cursor at row 1 column 1
	LCD_8bit_Command(LCD, _LCD_RETURN_HOME);
	// Send command to Display on and Blink cursor on
	LCD_8bit_Command(LCD, _LCD_UNDERLINE_CURSOR_ON);
	// Send command to set cursor increamenet toward right after writing without shift display
	LCD_8bit_Command(LCD, _LCD_INC_CURSOR_SHIFT_OFF);
	// Send command that i ready to recieve data so i make the AC (Address counter) to First address in DDRAM
	LCD_8bit_Command(LCD, _LCD_DDRAM_START);
}

void LCD_8bit_Set_Cursor(LCD_8bit_t *LCD, uint8_t row, uint8_t column)
{
	// Select row 1 or 2
	if ( (row == 2 || row == 1) && ((column > 0) && (column <= 16)) )
	{
		switch(row)
			{
			case 1:
				// Send command to Make AC (Address counter) point to The target column in row 1
				LCD_8bit_Command(LCD, _LCD_ROW1_START + --column);
				AddressDD = 0 + column;
				break;
			case 2:
				// Send command to Make AC (Address counter) point to The target column in row 2
				LCD_8bit_Command(LCD, _LCD_ROW2_START + --column);
				AddressDD = 16 + column;
				break;
			}
	}
}
void LCD_8bit_Command(LCD_8bit_t *LCD, uint8_t command)
{
	// To Tell LCD That i will send command Write RS LOW @ref INFO (LCD.H)
	GPIO_WRITE_PIN(&(LCD->RS), LOW);
	// Send 8bits Command on Wires Bit by Bit
	Send_8bit(LCD, command);
}

void LCD_8bit_Print_Char(LCD_8bit_t *LCD, uint8_t data){

	if (AddressDD == 16) 	LCD_8bit_Set_Cursor(LCD, 2, 1);
	else if (AddressDD == 32)
	{
		LCD_8bit_Set_Cursor(LCD, 1, 1);
		AddressDD = 0;
	}
	AddressDD++;
	// To Tell LCD That i will send Data Write RS HIGH @ref INFO (LCD.H)
	GPIO_WRITE_PIN(&(LCD->RS), HIGH);
	// Send 8bits data on Wires Bit by Bit
	Send_8bit(LCD, data);
}

void LCD_8bit_Print(LCD_8bit_t *LCD, uint8_t *data){
	while(*data) LCD_8bit_Print_Char(LCD, *data++);
}

void LCD_8bit_Print_Number(LCD_8bit_t *LCD, int value)
{
	uint8_t str[10] = {0};
	sprintf(str,"%i",value);
	LCD_8bit_Print(LCD, str);
}



void LCD_8bit_Print_Custom_char(LCD_8bit_t *LCD, const uint8_t c_char[], uint8_t Pos)
{
	Pos &= 0x07;
	LCD_8bit_Command(LCD, _LCD_CGRAM_START + (Pos*8));
	for(int i = 0; i < 8; i ++)
	{
		Send_CGRAM(LCD, c_char[i]);
	}
	if (AddressDD < 16) LCD_8bit_Command(LCD, (uint8_t)(_LCD_ROW1_START + AddressDD));
	else LCD_8bit_Command(LCD, (uint8_t)(_LCD_ROW2_START + (AddressDD - 16)));
	LCD_8bit_Print_Char(LCD, Pos);
}

static void Send_CGRAM(LCD_8bit_t *LCD, uint8_t data){
	// To Tell LCD That i will send Data Write RS HIGH @ref INFO (LCD.H)
	GPIO_WRITE_PIN(&(LCD->RS), HIGH);
	// Send 8bits data on Wires Bit by Bit
	Send_8bit(LCD, data);
}


static void Send_8bit(LCD_8bit_t *LCD, uint8_t _data_command)
{
	// Send 8bits data | command bit by bit
	for (char i = 0; i < 8; i++)
	{
		GPIO_WRITE_PIN( &(LCD->Data[(int)i]), ((_data_command >> (int)i) & 0x01) );
	}

	// Enable Signal to Recieve Data or Command  |_ Falling Edge
	GPIO_WRITE_PIN(&(LCD->EN), HIGH);
	for(volatile char delay = 0; delay < 255; delay++);  //Small delay i will write professional delay after learning it
	GPIO_WRITE_PIN(&(LCD->EN), LOW);
	for(volatile char delay = 0; delay < 255; delay++);
}

void LCD_8bit_Display_ON(LCD_8bit_t *LCD)
{
	// To Make Display ON on LDC
	LCD_8bit_Command(LCD, _LCD_DISPLAY_ON);
}

void LCD_8bit_Display_OFF(LCD_8bit_t *LCD)
{
	// To Make Display OFF Without losing DDRAM Content on LDC
	LCD_8bit_Command(LCD, _LCD_DISPLAY_OFF);
}

void LCD_8bit_Clear(LCD_8bit_t *LCD)
{
	// To Clear LCD & DDRAM Memory
	LCD_8bit_Command(LCD, _LCD_CLEAR);
}

#endif

static void myWait(volatile int ms)
{
	for (volatile int i = 0; i < ms; i++) for (volatile int j = 0; j < 255; j++);

}





