/**
 ******************************************************************************
 * @File           : SevenSegment.c
 * @Author         : Abdelrhman Marzoq
 * @Brief          : Sevensegment Driver
 * 			Created on: Jul 26, 2024
 ******************************************************************************
**/

#include "SevenSegment.h"


void SevenSegment_init(_7Segment_t *Segment)
{
	Segment->A.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	Segment->A.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
	GPIO_INIT(&(Segment->A));

	Segment->B.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	Segment->B.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
	GPIO_INIT(&(Segment->B));

	Segment->C.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	Segment->C.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
	GPIO_INIT(&(Segment->C));


	Segment->D.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	Segment->D.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
	GPIO_INIT(&(Segment->D));


	Segment->E.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	Segment->E.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
	GPIO_INIT(&(Segment->E));


	Segment->F.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	Segment->F.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
	GPIO_INIT(&(Segment->F));


	Segment->G.GPIO_MODE = GPIO_MODE_OUTPUT_PP;
	Segment->G.GPIO_OUTPUT_Speed = GPIO_SPEED_2M;
	GPIO_INIT(&(Segment->G));

}

void SevenSegment_Print(_7Segment_t *Segment, uint8_t value)
{
	value &= ~(0xF0);
    switch(value)
    {
	    case 0:
	        GPIO_WRITE_PIN(&(Segment->A), HIGH);
	        GPIO_WRITE_PIN(&(Segment->B), HIGH);
	        GPIO_WRITE_PIN(&(Segment->C), HIGH);
	        GPIO_WRITE_PIN(&(Segment->D), HIGH);
	        GPIO_WRITE_PIN(&(Segment->E), HIGH);
	        GPIO_WRITE_PIN(&(Segment->F), HIGH);
	        GPIO_WRITE_PIN(&(Segment->G), LOW);
	        break;
	    case 1:
	        GPIO_WRITE_PIN(&(Segment->A), LOW);
	        GPIO_WRITE_PIN(&(Segment->B), HIGH);
	        GPIO_WRITE_PIN(&(Segment->C), HIGH);
	        GPIO_WRITE_PIN(&(Segment->D), LOW);
	        GPIO_WRITE_PIN(&(Segment->E), LOW);
	        GPIO_WRITE_PIN(&(Segment->F), LOW);
	        GPIO_WRITE_PIN(&(Segment->G), LOW);
	        break;
	    case 2:
	        GPIO_WRITE_PIN(&(Segment->A), HIGH);
	        GPIO_WRITE_PIN(&(Segment->B), HIGH);
	        GPIO_WRITE_PIN(&(Segment->C), LOW);
	        GPIO_WRITE_PIN(&(Segment->D), HIGH);
	        GPIO_WRITE_PIN(&(Segment->E), HIGH);
	        GPIO_WRITE_PIN(&(Segment->F), LOW);
	        GPIO_WRITE_PIN(&(Segment->G), HIGH);
	        break;
	    case 3:
	        GPIO_WRITE_PIN(&(Segment->A), HIGH);
	        GPIO_WRITE_PIN(&(Segment->B), HIGH);
	        GPIO_WRITE_PIN(&(Segment->C), HIGH);
	        GPIO_WRITE_PIN(&(Segment->D), HIGH);
	        GPIO_WRITE_PIN(&(Segment->E), LOW);
	        GPIO_WRITE_PIN(&(Segment->F), LOW);
	        GPIO_WRITE_PIN(&(Segment->G), HIGH);
	        break;
	    case 4:
	        GPIO_WRITE_PIN(&(Segment->A), LOW);
	        GPIO_WRITE_PIN(&(Segment->B), HIGH);
	        GPIO_WRITE_PIN(&(Segment->C), HIGH);
	        GPIO_WRITE_PIN(&(Segment->D), LOW);
	        GPIO_WRITE_PIN(&(Segment->E), LOW);
	        GPIO_WRITE_PIN(&(Segment->F), HIGH);
	        GPIO_WRITE_PIN(&(Segment->G), HIGH);
	        break;
	    case 5:
	        GPIO_WRITE_PIN(&(Segment->A), HIGH);
	        GPIO_WRITE_PIN(&(Segment->B), LOW);
	        GPIO_WRITE_PIN(&(Segment->C), HIGH);
	        GPIO_WRITE_PIN(&(Segment->D), HIGH);
	        GPIO_WRITE_PIN(&(Segment->E), LOW);
	        GPIO_WRITE_PIN(&(Segment->F), HIGH);
	        GPIO_WRITE_PIN(&(Segment->G), HIGH);
	        break;
	    case 6:
	        GPIO_WRITE_PIN(&(Segment->A), HIGH);
	        GPIO_WRITE_PIN(&(Segment->B), LOW);
	        GPIO_WRITE_PIN(&(Segment->C), HIGH);
	        GPIO_WRITE_PIN(&(Segment->D), HIGH);
	        GPIO_WRITE_PIN(&(Segment->E), HIGH);
	        GPIO_WRITE_PIN(&(Segment->F), HIGH);
	        GPIO_WRITE_PIN(&(Segment->G), HIGH);
	        break;
	    case 7:
	        GPIO_WRITE_PIN(&(Segment->A), HIGH);
	        GPIO_WRITE_PIN(&(Segment->B), HIGH);
	        GPIO_WRITE_PIN(&(Segment->C), HIGH);
	        GPIO_WRITE_PIN(&(Segment->D), LOW);
	        GPIO_WRITE_PIN(&(Segment->E), LOW);
	        GPIO_WRITE_PIN(&(Segment->F), LOW);
	        GPIO_WRITE_PIN(&(Segment->G), LOW);
	        break;
	    case 8:
	        GPIO_WRITE_PIN(&(Segment->A), HIGH);
	        GPIO_WRITE_PIN(&(Segment->B), HIGH);
	        GPIO_WRITE_PIN(&(Segment->C), HIGH);
	        GPIO_WRITE_PIN(&(Segment->D), HIGH);
	        GPIO_WRITE_PIN(&(Segment->E), HIGH);
	        GPIO_WRITE_PIN(&(Segment->F), HIGH);
	        GPIO_WRITE_PIN(&(Segment->G), HIGH);
	        break;
	    case 9:
	        GPIO_WRITE_PIN(&(Segment->A), HIGH);
	        GPIO_WRITE_PIN(&(Segment->B), HIGH);
	        GPIO_WRITE_PIN(&(Segment->C), HIGH);
	        GPIO_WRITE_PIN(&(Segment->D), HIGH);
	        GPIO_WRITE_PIN(&(Segment->E), LOW);
	        GPIO_WRITE_PIN(&(Segment->F), HIGH);
	        GPIO_WRITE_PIN(&(Segment->G), HIGH);
	        break;
	    default:
	        // Handle invalid values, perhaps turn off all segments
	        GPIO_WRITE_PIN(&(Segment->A), LOW);
	        GPIO_WRITE_PIN(&(Segment->B), LOW);
	        GPIO_WRITE_PIN(&(Segment->C), LOW);
	        GPIO_WRITE_PIN(&(Segment->D), LOW);
	        GPIO_WRITE_PIN(&(Segment->E), LOW);
	        GPIO_WRITE_PIN(&(Segment->F), LOW);
	        GPIO_WRITE_PIN(&(Segment->G), LOW);
	        break;
    }
}
