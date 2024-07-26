/*
 * SevenSegment.h
 *
 *  Created on: Jul 26, 2024
 *      Author: Abdelrhman Marzoq
 */

#ifndef _7SEG_SEVENSEGMENT_H_
#define _7SEG_SEVENSEGMENT_H_

#include "../STM32F103C6_MCAL_Drivers/GPIO/STM32F103C6_GPIO_Driver.h"

typedef struct
{
	GPIO_PinConfig_t A;
	GPIO_PinConfig_t B;
	GPIO_PinConfig_t C;
	GPIO_PinConfig_t D;
	GPIO_PinConfig_t E;
	GPIO_PinConfig_t F;
	GPIO_PinConfig_t G;
}_7Segment_t;


void SevenSegment_init(_7Segment_t *Segment);
void SevenSegment_Print(_7Segment_t *Segment, uint8_t value);


#endif /* _7SEG_SEVENSEGMENT_H_ */
