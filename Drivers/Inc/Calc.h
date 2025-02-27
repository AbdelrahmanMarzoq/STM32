/**
 ******************************************************************************
 * @File           : Calc.h
 * @Author         : Abdelrhman Marzoq
 * @Brief          : Calculater Header File
 * 			Created on: Jul 29, 2024
 ******************************************************************************
**/

#ifndef CALC_H_
#define CALC_H_

#include "../HAL_Drivers/LCD/LCD.h"
#include "../HAL_Drivers/Keypad/Keypad.h"
#include "../STM32F103C6_MCAL_Drivers/EXTI/EXTI.h"

#define MAX 100

// externed Objects
extern LCD_4bit_t lcd;
extern Keypad_t key;
extern EXIT_t exti;

extern uint8_t i;
extern volatile uint8_t ISR;

extern uint8_t Operation[60];

typedef struct
{
	uint8_t Data[MAX];
	int Top;
}Stack_Char;

typedef struct
{
	float Data[MAX];
	int Top;
}Stack_Float;

void Calc_init(void);
void Calc(uint8_t *Equ);


void Reset(void);

#endif /* CALC_H_ */
