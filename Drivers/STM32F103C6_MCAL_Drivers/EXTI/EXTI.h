/*
 * EXTI.h
 *
 *  Created on: Jul 27, 2024
 *      Author: Abdelrhman Marzoq
 *
 *
 * You must Enable Clock RCC For AFIO in your Layer To Work Proberly
 *
 *
 */

// To Enable AFIO Just Write this Defined Macro (RCC_AFIO_CLK_EN)


#ifndef EXTI_EXTI_H_
#define EXTI_EXTI_H_


						/*****************************************/
						/*             Include Section           */
						/*****************************************/
#include "../STM32F103C6_MCAL_Drivers/GPIO/STM32F103C6_GPIO_Driver.h"




						/*****************************************/
						/*             Typedef Section           */
						/*****************************************/

typedef struct
{
	GPIO_Periphral 	*GPIOx;
	uint16_t  		Pin_Number;
	uint8_t 		EXTI_Line;
	uint8_t			EXTI_IRQ;
}Config_t;

typedef struct
{
	Config_t	 EXTI_PIN;      // Specify the Configuration for EXTI
							    // This Parameter must be based on @ref EXTI_Define

	void(*PF_IRQ)(void);        // Address of Function() Will callback once the IRQ Happen

	uint8_t 	Detect_EXTI;    // Specify the Configuration how to detect EXTI
							    // This Parameter must be based on @ref Trigger_Define


	uint8_t     IRQ_EN;         // This Var For IF EXTI Need to Enable or Disable
								// This Parameter must be based on @ref ENABLE_Define



}EXIT_t;


						/*****************************************/
						/*             Refrence Section          */
						/*****************************************/

//@ref ENABLE_Define
#define EN_EXTI             0      // For Enable EXTI
#define DIS_EXTI			1	   // For Disable EXTI

// @ref Trigger_Define
#define RISING              0      // For Detecting Rising Edge
#define FALLING             1      // For Detecting Falling Edge
#define RISING_FALLING      3 	   // For Detecting Both Falling&Rising Edge

// @ref EXTI_Define
//                           EXTI0
#define EXTI0PA0            (Config_t){GPIOA, GPIO_PIN0, EXTI0, EXTI0_IRQ}
#define EXTI0PB0            (Config_t){GPIOB, GPIO_PIN0, EXTI0, EXTI0_IRQ}
#define EXTI0PC0            (Config_t){GPIOC, GPIO_PIN0, EXTI0, EXTI0_IRQ}
#define EXTI0PD0            (Config_t){GPIOD, GPIO_PIN0, EXTI0, EXTI0_IRQ}

// EXTI1
#define EXTI1PA1            (Config_t){GPIOA, GPIO_PIN1, EXTI1, EXTI1_IRQ}
#define EXTI1PB1            (Config_t){GPIOB, GPIO_PIN1, EXTI1, EXTI1_IRQ}
#define EXTI1PC1            (Config_t){GPIOC, GPIO_PIN1, EXTI1, EXTI1_IRQ}
#define EXTI1PD1            (Config_t){GPIOD, GPIO_PIN1, EXTI1, EXTI1_IRQ}

// EXTI2
#define EXTI2PA2            (Config_t){GPIOA, GPIO_PIN2, EXTI2, EXTI2_IRQ}
#define EXTI2PB2            (Config_t){GPIOB, GPIO_PIN2, EXTI2, EXTI2_IRQ}
#define EXTI2PC2            (Config_t){GPIOC, GPIO_PIN2, EXTI2, EXTI2_IRQ}
#define EXTI2PD2            (Config_t){GPIOD, GPIO_PIN2, EXTI2, EXTI2_IRQ}

// EXTI3
#define EXTI3PA3            (Config_t){GPIOA, GPIO_PIN3, EXTI3, EXTI3_IRQ}
#define EXTI3PB3            (Config_t){GPIOB, GPIO_PIN3, EXTI3, EXTI3_IRQ}
#define EXTI3PC3            (Config_t){GPIOC, GPIO_PIN3, EXTI3, EXTI3_IRQ}
#define EXTI3PD3            (Config_t){GPIOD, GPIO_PIN3, EXTI3, EXTI3_IRQ}

// EXTI4
#define EXTI4PA4            (Config_t){GPIOA, GPIO_PIN4, EXTI4, EXTI4_IRQ}
#define EXTI4PB4            (Config_t){GPIOB, GPIO_PIN4, EXTI4, EXTI4_IRQ}
#define EXTI4PC4            (Config_t){GPIOC, GPIO_PIN4, EXTI4, EXTI4_IRQ}
#define EXTI4PD4            (Config_t){GPIOD, GPIO_PIN4, EXTI4, EXTI4_IRQ}

// EXTI5
#define EXTI5PA5            (Config_t){GPIOA, GPIO_PIN5, EXTI5, EXTI5_IRQ}
#define EXTI5PB5            (Config_t){GPIOB, GPIO_PIN5, EXTI5, EXTI5_IRQ}
#define EXTI5PC5            (Config_t){GPIOC, GPIO_PIN5, EXTI5, EXTI5_IRQ}
#define EXTI5PD5            (Config_t){GPIOD, GPIO_PIN5, EXTI5, EXTI5_IRQ}

// EXTI6
#define EXTI6PA6            (Config_t){GPIOA, GPIO_PIN6, EXTI6, EXTI6_IRQ}
#define EXTI6PB6            (Config_t){GPIOB, GPIO_PIN6, EXTI6, EXTI6_IRQ}
#define EXTI6PC6            (Config_t){GPIOC, GPIO_PIN6, EXTI6, EXTI6_IRQ}
#define EXTI6PD6            (Config_t){GPIOD, GPIO_PIN6, EXTI6, EXTI6_IRQ}

// EXTI7
#define EXTI7PA7            (Config_t){GPIOA, GPIO_PIN7, EXTI7, EXTI7_IRQ}
#define EXTI7PB7            (Config_t){GPIOB, GPIO_PIN7, EXTI7, EXTI7_IRQ}
#define EXTI7PC7            (Config_t){GPIOC, GPIO_PIN7, EXTI7, EXTI7_IRQ}
#define EXTI7PD7            (Config_t){GPIOD, GPIO_PIN7, EXTI7, EXTI7_IRQ}

// EXTI8
#define EXTI8PA8            (Config_t){GPIOA, GPIO_PIN8, EXTI8, EXTI8_IRQ}
#define EXTI8PB8            (Config_t){GPIOB, GPIO_PIN8, EXTI8, EXTI8_IRQ}
#define EXTI8PC8            (Config_t){GPIOC, GPIO_PIN8, EXTI8, EXTI8_IRQ}
#define EXTI8PD8            (Config_t){GPIOD, GPIO_PIN8, EXTI8, EXTI8_IRQ}

// EXTI9
#define EXTI9PA9            (Config_t){GPIOA, GPIO_PIN9, EXTI9, EXTI9_IRQ}
#define EXTI9PB9            (Config_t){GPIOB, GPIO_PIN9, EXTI9, EXTI9_IRQ}
#define EXTI9PC9            (Config_t){GPIOC, GPIO_PIN9, EXTI9, EXTI9_IRQ}
#define EXTI9PD9            (Config_t){GPIOD, GPIO_PIN9, EXTI9, EXTI9_IRQ}

// EXTI10
#define EXTI10PA10          (Config_t){GPIOA, GPIO_PIN10, EXTI10, EXTI10_IRQ}
#define EXTI10PB10          (Config_t){GPIOB, GPIO_PIN10, EXTI10, EXTI10_IRQ}
#define EXTI10PC10          (Config_t){GPIOC, GPIO_PIN10, EXTI10, EXTI10_IRQ}
#define EXTI10PD10          (Config_t){GPIOD, GPIO_PIN10, EXTI10, EXTI10_IRQ}

// EXTI11
#define EXTI11PA11          (Config_t){GPIOA, GPIO_PIN11, EXTI11, EXTI11_IRQ}
#define EXTI11PB11          (Config_t){GPIOB, GPIO_PIN11, EXTI11, EXTI11_IRQ}
#define EXTI11PC11          (Config_t){GPIOC, GPIO_PIN11, EXTI11, EXTI11_IRQ}
#define EXTI11PD11          (Config_t){GPIOD, GPIO_PIN11, EXTI11, EXTI11_IRQ}

// EXTI12
#define EXTI12PA12          (Config_t){GPIOA, GPIO_PIN12, EXTI12, EXTI12_IRQ}
#define EXTI12PB12          (Config_t){GPIOB, GPIO_PIN12, EXTI12, EXTI12_IRQ}
#define EXTI12PC12          (Config_t){GPIOC, GPIO_PIN12, EXTI12, EXTI12_IRQ}
#define EXTI12PD12          (Config_t){GPIOD, GPIO_PIN12, EXTI12, EXTI12_IRQ}

// EXTI13
#define EXTI13PA13          (Config_t){GPIOA, GPIO_PIN13, EXTI13, EXTI13_IRQ}
#define EXTI13PB13          (Config_t){GPIOB, GPIO_PIN13, EXTI13, EXTI13_IRQ}
#define EXTI13PC13          (Config_t){GPIOC, GPIO_PIN13, EXTI13, EXTI13_IRQ}
#define EXTI13PD13          (Config_t){GPIOD, GPIO_PIN13, EXTI13, EXTI13_IRQ}

// EXTI14
#define EXTI14PA14          (Config_t){GPIOA, GPIO_PIN14, EXTI14, EXTI14_IRQ}
#define EXTI14PB14          (Config_t){GPIOB, GPIO_PIN14, EXTI14, EXTI14_IRQ}
#define EXTI14PC14          (Config_t){GPIOC, GPIO_PIN14, EXTI14, EXTI14_IRQ}
#define EXTI14PD14          (Config_t){GPIOD, GPIO_PIN14, EXTI14, EXTI14_IRQ}

// EXTI15
#define EXTI15PA15          (Config_t){GPIOA, GPIO_PIN15, EXTI15, EXTI15_IRQ}
#define EXTI15PB15          (Config_t){GPIOB, GPIO_PIN15, EXTI15, EXTI15_IRQ}
#define EXTI15PC15          (Config_t){GPIOC, GPIO_PIN15, EXTI15, EXTI15_IRQ}
#define EXTI15PD15          (Config_t){GPIOD, GPIO_PIN15, EXTI15, EXTI15_IRQ}



						/*****************************************/
						/*             Macros Section            */
						/*****************************************/

//                  		EXTI Lines
#define EXTI0               0
#define EXTI1               1
#define EXTI2               2
#define EXTI3               3
#define EXTI4               4
#define EXTI5               5
#define EXTI6               6
#define EXTI7               7
#define EXTI8               8
#define EXTI9               9
#define EXTI10              10
#define EXTI11              11
#define EXTI12              12
#define EXTI13              13
#define EXTI14              14
#define EXTI15              15


						/*****************************************/
						/*             Function Declaration      */
						/*****************************************/

/**================================================================
 * @Fn                 	- EXTI_Init
 * @brief				- Configure The EXTI Peripheral
 * @param [in]			- EXTI_Config : Is pointer to a struct Has Info for Your
 * 					      (Detect Edge) (En or Dis EXTI) (Pin&Port) (Ptr Function to Call Back).
 * @retval				- NONE
 * Note					- First Fun You need to call After Implement an object of EXIT_t
*/
void EXTI_Init(EXIT_t *EXTI_Config);

/**================================================================
 * @Fn                 	- EXTI_Update
 * @brief				- Configure The EXTI Peripheral
 * @param [in]			- EXTI_Config : Is pointer to a struct Has Info for Your
 *  			          (Detect Edge) (En or Dis EXTI) (Pin&Port) (Ptr Function to Call Back).
 * @retval				- NONE
 * Note					- If You Need Update in Pin Like Disable EXTI
 * 						  change Detected Interrupt Edge in Run Time For Readiablity Use This Function.
*/
void EXTI_Update(EXIT_t *EXTI_Config);

/**================================================================
 * @Fn                 	- EXTI_DeInit
 * @brief				- reset all EXTI Register
 * @param [in]			- NONE
 * @retval				- NONE
 * Note					- NONE
*/
void EXTI_DeInit(void);




#endif /* EXTI_EXTI_H_ */
