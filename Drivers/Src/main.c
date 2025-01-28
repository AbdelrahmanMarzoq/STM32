/**
 ******************************************************************************
 * @File           : main.c
 * @Author         : Abdelrhman Marzoq
 * @Brief          : Main program body
 ******************************************************************************
 **/

#include "../STM32F103C6_MCAL_Drivers/UART/STM32F103C6_UART_Driver.h"
#include "../STM32F103C6_MCAL_Drivers/GPIO/STM32F103C6_GPIO_Driver.h"
#include "../STM32F103C6_MCAL_Drivers/SPI/STM32F103C6_SPI_Driver.h"
#include "../STM32F103C6_MCAL_Drivers/I2C/STM32F103C6_I2C_Driver.h"
#include "../STM32F103C6_MCAL_Drivers/EXTI/EXTI.h"
#include "../HAL_Drivers/EEPROM/EEPROM.h"
#include "../HAL_Drivers/LCD/LCD.h"









int main()
{


//	RCC_GPIOA_CLK_EN;
//	RCC_GPIOB_CLK_EN;
//	RCC_AFIO_CLK_EN;

//	init_Func();

	unsigned char TX_BUFFER[2] = {5,6};
	unsigned char RX_BUFFER[2];

	EEPROM_init();
	uint16_t y = 0x9595;
	EEPROM_Write(y, TX_BUFFER, 2);
	EEPROM_Read(y, RX_BUFFER, 2);


	while (1)
	{
	}

	return 0;
}













//
//#define STACK_ALLIGN		0x8
//#define STACK_SIZE_TASKB	0x100
//#define STACK_SIZE_TASKA	0x100
//#define MAIN_STACK_SIZE		0x500
//
//
//#define OS_SET_PSP(add) 		__asm("mov r0, %0 \n\t msr PSP,r0" : : "r" (add))
//#define OS_SP_TO_PSP			__asm("mrs r0, CONTROL \n\t ORR r0,r0,#0x02 \n\t msr CONTROL, r0")
//#define OS_SP_TO_MSP			__asm("mrs r0, CONTROL \n\t AND r0,r0,#0x05 \n\t msr CONTROL, r0")
//
//#define OS_GENERATE_EXCEPTION	__asm("SVC #0x3")
//
//#define SWITCH_CPU_TO_PRIV		__asm("mrs r0, CONTROL \n\t LSR r0, r0,#1 \n\t LSL r0, r0,#1  \n\t msr CONTROL, r0")
//#define SWITCH_CPU_TO_UNPRIV	__asm("mrs r0, CONTROL \n\t ORR r0, r0, #0x1 \n\t msr CONTROL, r0")
//
//
//
//extern _estack;
//
//
//unsigned int S_MSP   =		(unsigned int)&_estack;
//
//unsigned int E_MSP;
//unsigned int S_TASKA;
//unsigned int E_TASKA;
//unsigned int S_TASKB;
//unsigned int E_TASKB;
//
//
//
//EXIT_t ext;
//unsigned char FLAG_A = 0;
//unsigned char FLAG_B = 0;
//int IRQ_FLAG = 0;
//
//void EXT_Callback(void);
//unsigned int TASKA(int a, int b, int c);
//unsigned int TASKB(int a, int b, int c, int d);
//
//
//
//
//void init_Func()
//{
//	ext.Detect_EXTI = RISING;
//	ext.EXTI_PIN 	= EXTI9PB9;
//	ext.IRQ_EN 		= EN_EXTI;
//	ext.PF_IRQ		= EXT_Callback;
//	EXTI_Init(&ext);
//
////	TASKA(1, 2, 3);
//}
//
//
//
//
//static void myWait(volatile int ms)
//{
//	for (volatile int i = 0; i < ms; i++)
//	{
//		for (volatile int j = 0; j < 255; j++);
//	}
//}
//
//
//
//unsigned int TASKA(int a, int b, int c)
//{
//	TASKB(1, 2, 3, 4);
//	return a+b+c;
//}
//
//unsigned int TASKB(int a, int b, int c, int d)
//{
//	return a+b+c+d;
//}
//
//void MainOS()
//{
//	E_MSP	 =		(S_MSP - MAIN_STACK_SIZE);
//
//
//	S_TASKA =		(E_MSP   - STACK_ALLIGN);
//	E_TASKA =		(S_TASKA - STACK_SIZE_TASKA);
//
//
//	S_TASKB =		(E_TASKA - STACK_ALLIGN);
//	E_TASKB = 		(S_TASKB - STACK_SIZE_TASKB);
//
//}
//
//void OS_SVC(unsigned int *Stack_Frame)
//{
//	unsigned char SVC_Number;
//	unsigned int val1,val2;
//
//	SVC_Number = *((unsigned char *)(((unsigned char *)Stack_Frame[6]) - 2));
//
//	val1 = Stack_Frame[0];
//	val2 = Stack_Frame[1];
//	switch(SVC_Number)
//	{
//	case 1:
//		Stack_Frame[0] = val1 + val2;
//		break;
//	case 2:
//		Stack_Frame[0] = val1 - val2;
//		break;
//	case 3:
//		Stack_Frame[0] = val1 * val2;
//		break;
//	}
//
//}
//
//__attribute((naked)) void SVC_Handler()
//{
//	__asm(  "tst lr, #4 	\n\t"
//			"ITE EQ 		\n\t"
//			"mrseq  r0, MSP \n\t"
//			"mrsne  r0, PSP \n\t"
//			"B OS_SVC");
//}
//
//int OS_SVC_Set(int a, int b, int ID)
//{
//	int result;
//	switch (ID)
//	{
//	case 1:
//		__asm("svc #1");
//		break;
//	case 2:
//		__asm("svc #2");
//		break;
//	case 3:
//		__asm("svc #3");
//		break;
//	}
//	__asm("mov %0,r0":"=r"(result));
//	return result;
//}
//
//
//
//int main(void)
//{
//
//	/*
//	RCC_GPIOA_CLK_EN;
//	RCC_GPIOB_CLK_EN;
//	RCC_AFIO_CLK_EN;
//
//	init_Func();
//
//	int x = 5;
//	void (*PTR)(void) = EXT_Callback;
//
////	__asm("mov r0, %[IN]"
////			:
////			: [IN] "r" (PTR));
////	__asm("LDR r0, [%[IN]]"
////			:
////			: [IN] "r" (PTR));
//
//
////	TASKA(1, 2, 3);
//	int z;
//	z = Print();*/
//
//
//
//
//	int result;
//	result = OS_SVC_Set(2,2,1);
//	result = OS_SVC_Set(2,2,2);
//	result = OS_SVC_Set(2,2,3);
//
//
//	while(1)
//	{
//
//	}
//}
//
//
//
//
//
//
//int except(int y, int z, int f)
//{
//	int x = 5;
//	__asm("SVC #3");
//	return 0;
//}
//
//
//
//
//
//
//
//
//
//int Print()
//{
//	int z = 5;
//	__asm("mov r0, %[in]"
//			:
//			:[in] "r" (z));
//}
//
//void EXT_Callback(void)
//{
//	if (IRQ_FLAG == 1)
//	{
//		IRQ_FLAG = 0;
//		FLAG_A = 1;
//	}
//	else if (IRQ_FLAG == 0)
//	{
//		IRQ_FLAG = 1;
//		FLAG_B = 1;
//	}
//
//}

