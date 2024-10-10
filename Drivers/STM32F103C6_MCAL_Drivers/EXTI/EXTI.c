/**
 ******************************************************************************
 * @File           : EXTI.c
 * @Author         : Abdelrhman Marzoq
 * @Brief          : External interrupt Driver
 * 			Created on: Jul 27, 2024
 ******************************************************************************
**/


						/*****************************************/
						/*             Include Section           */
						/*****************************************/
#include "EXTI.h"



#define EXTI_PA     0
#define EXTI_PB     1
#define EXTI_PC     2
#define EXTI_PD     3


#define AFIO_Mapping(x)     (x == GPIOA)? EXTI_PA:\
							(x == GPIOB)? EXTI_PB:\
							(x == GPIOC)? EXTI_PC:\
							(x == GPIOD)? EXTI_PD:0


static void DIS_NVIC(uint8_t Line);
static void EN_NVIC(uint8_t Line);



						/*****************************************/
						/*           Function Definition         */
						/*****************************************/


static void (* GP_IRQ[15])(void);


static void Update(EXIT_t *EXTI_Config)
{
	// 1- GPIO Init to be AF Input
	GPIO_PinConfig_t EXTI_PIN;
	EXTI_PIN.GPIOx = EXTI_Config->EXTI_PIN.GPIOx;
	EXTI_PIN.GPIO_PinNumber = EXTI_Config->EXTI_PIN.Pin_Number;
	EXTI_PIN.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
	EXTI_PIN.GPIO_MODE = GPIO_MODE_INPUT_AF;
	GPIO_INIT(&EXTI_PIN);
	//-------------------------------------------

	// 2- Configure AFIO To Rout to EXTI Line With Portx
	// To get The Reg in Array without If Condition
	uint8_t AFIO_Index = (EXTI_Config->EXTI_PIN.EXTI_Line / 4);
	// To Get the Pos in Reg that i will write on it without if condition
	uint8_t AFIO_Bit_Pos = ((EXTI_Config->EXTI_PIN.EXTI_Line % 4) * 4);
	// Clear EXTICRx Reg to write on it
	AFIO->EXTICR[AFIO_Index] &= ~(0XF << AFIO_Bit_Pos);
	// Writing On EXTICR Reg for Routing Pin as EXTI Line
	AFIO->EXTICR[AFIO_Index] |= ((AFIO_Mapping(EXTI_Config->EXTI_PIN.GPIOx) & 0xF) << AFIO_Bit_Pos);
	//-------------------------------------------

	// 3- Update Rising or Falling Edge or both
	// Clearing it to Customize the function the user will choose
	EXTI->RTSR &= ~(1<< EXTI_Config->EXTI_PIN.EXTI_Line);
	EXTI->FTSR &= ~(1<< EXTI_Config->EXTI_PIN.EXTI_Line);

	if ( EXTI_Config->Detect_EXTI == RISING)
	{
		EXTI->RTSR |= (1<< EXTI_Config->EXTI_PIN.EXTI_Line);
	}
	else if (EXTI_Config->Detect_EXTI == FALLING)
	{
		EXTI->FTSR |= (1<< EXTI_Config->EXTI_PIN.EXTI_Line);
	}
	else if (EXTI_Config->Detect_EXTI == RISING_FALLING)
	{
		EXTI->FTSR |= (1<< EXTI_Config->EXTI_PIN.EXTI_Line);
		EXTI->RTSR |= (1<< EXTI_Config->EXTI_PIN.EXTI_Line);
	}
	//-------------------------------------------

	// 4- Store the address of Function Call back you need to execute while EXTI Line happen
	GP_IRQ[EXTI_Config->EXTI_PIN.EXTI_Line] = EXTI_Config->PF_IRQ;
	//-------------------------------------------

	// 5- Enable Mask for NVIC And EXTI to Receive IRQ
	if (EXTI_Config->IRQ_EN == EN_EXTI)
	{
		EXTI->IMR |= (1 << EXTI_Config->EXTI_PIN.EXTI_Line);
		EN_NVIC(EXTI_Config->EXTI_PIN.EXTI_Line);
	}
	else if (EXTI_Config->IRQ_EN == DIS_EXTI)
	{
		EXTI->IMR &= ~(1 << EXTI_Config->EXTI_PIN.EXTI_Line);
		DIS_NVIC(EXTI_Config->EXTI_PIN.EXTI_Line);
	}
	//-------------------------------------------
}

void EXTI_Init(EXIT_t *EXTI_Config)
{
	Update(EXTI_Config);
}

void EXTI_Update(EXIT_t *EXTI_Config)
{
	Update(EXTI_Config);
}

void EXTI_DeInit(void)
{
	// Reset Reg to its reset value in EXTI Periphral
	EXTI->EMR   = 0x00000000;
	EXTI->IMR   = 0x00000000;
	EXTI->RTSR  = 0x00000000;
	EXTI->FTSR  = 0x00000000;
	EXTI->SWIER = 0x00000000;
	// PR is Panding Reg in EXTI Peripheral to clear it you need to write 1 from Data Sheet rc_w1
	EXTI->PR    = 0xFFFFFFFF;

	// Disable Mask on NVIC
	NVIC_Disable_EXTI0;
	NVIC_Disable_EXTI1;
	NVIC_Disable_EXTI2;
	NVIC_Disable_EXTI3;
	NVIC_Disable_EXTI4;
	NVIC_Disable_EXTI5_9;
	NVIC_Disable_EXTI10_15;
}

static void EN_NVIC(uint8_t Line)
{
	switch (Line)
	{
	case 0:
		NVIC_Enable_EXTI0;break;
	case 1:
		NVIC_Enable_EXTI1;break;
	case 3:
		NVIC_Enable_EXTI3;break;
	case 4:
		NVIC_Enable_EXTI4;break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		NVIC_Enable_EXTI5_9;break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_Enable_EXTI10_15;break;
	default: break;
	}
}

static void DIS_NVIC(uint8_t Line)
{
	switch (Line)
	{
	case 0:
		NVIC_Disable_EXTI0;break;
	case 1:
		NVIC_Disable_EXTI1;break;
	case 3:
		NVIC_Disable_EXTI3;break;
	case 4:
		NVIC_Disable_EXTI4;break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		NVIC_Disable_EXTI5_9;break;
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		NVIC_Disable_EXTI10_15;break;
	default: break;
	}
}

void EXTI0_IRQHandler(void)
{
	// While Execution this function Means that NVIC Receive Ack signal from CPU
	// Disable Pending Request From EXTI Peripheral And Calling Your Provided Function
	EXTI->PR |= 1<<EXTI0;
	GP_IRQ[0]();
}

void EXTI1_IRQHandler(void)
{
	// While Execution this function Means that NVIC Receive Ack signal from CPU
	// Disable Pending Request From EXTI Peripheral And Calling Your Provided Function
	EXTI->PR |= 1<<EXTI1;
	GP_IRQ[1]();
}

void EXTI2_IRQHandler(void)
{
	// While Execution this function Means that NVIC Receive Ack signal from CPU
	// Disable Pending Request From EXTI Peripheral And Calling Your Provided Function
	EXTI->PR |= 1<<EXTI2;
	GP_IRQ[2]();
}

void EXTI3_IRQHandler(void)
{
	// While Execution this function Means that NVIC Receive Ack signal from CPU
	// Disable Pending Request From EXTI Peripheral And Calling Your Provided Function
	EXTI->PR |= 1<<EXTI3;
	GP_IRQ[3]();
}

void EXTI4_IRQHandler(void)
{
	// While Execution this function Means that NVIC Receive Ack signal from CPU
	// Disable Pending Request From EXTI Peripheral And Calling Your Provided Function
	EXTI->PR |= 1<<EXTI4;
	GP_IRQ[4]();
}

void EXTI9_5_IRQHandler(void)
{
	// While Execution this function Means that NVIC Receive Ack signal from CPU
	// Disable Pending Request From EXTI Peripheral And Calling Your Provided Function
	if (EXTI->PR & 1<<5){	EXTI->PR |= 1<<EXTI5;	GP_IRQ[5]();}
	if (EXTI->PR & 1<<6){	EXTI->PR |= 1<<EXTI6;	GP_IRQ[6]();}
	if (EXTI->PR & 1<<7){	EXTI->PR |= 1<<EXTI7;	GP_IRQ[7]();}
	if (EXTI->PR & 1<<8){	EXTI->PR |= 1<<EXTI8;	GP_IRQ[8]();}
	if (EXTI->PR & 1<<9){	EXTI->PR |= 1<<EXTI9;	GP_IRQ[9]();}
}

void EXTI15_10_IRQHandler(void)
{
	// While Execution this function Means that NVIC Receive Ack signal from CPU
	// Disable Pending Request From EXTI Peripheral And Calling Your Provided Function
	if (EXTI->PR & 1<<10){	EXTI->PR |= 1<<EXTI10;	GP_IRQ[10]();}
	if (EXTI->PR & 1<<11){	EXTI->PR |= 1<<EXTI11;	GP_IRQ[11]();}
	if (EXTI->PR & 1<<12){	EXTI->PR |= 1<<EXTI12;	GP_IRQ[12]();}
	if (EXTI->PR & 1<<13){	EXTI->PR |= 1<<EXTI13;	GP_IRQ[13]();}
	if (EXTI->PR & 1<<14){	EXTI->PR |= 1<<EXTI14;	GP_IRQ[14]();}
	if (EXTI->PR & 1<<15){	EXTI->PR |= 1<<EXTI15;	GP_IRQ[15]();}
}
