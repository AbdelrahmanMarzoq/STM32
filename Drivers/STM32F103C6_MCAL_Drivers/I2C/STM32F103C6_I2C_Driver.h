/*
 * STM32F103C6_I2C_Driver.h
 *
 *  Created on: Jan 27, 2025
 *      Author: Abdelrhman Marzoq
 */

#ifndef I2C_STM32F103C6_I2C_DRIVER_H_
#define I2C_STM32F103C6_I2C_DRIVER_H_


#include "../STM32F103X6.h"
#include "../STM32F103C6_MCAL_Drivers/GPIO/STM32F103C6_GPIO_Driver.h"
#include "../STM32F103C6_MCAL_Drivers/RCC/STM32F103C6_RCC_Driver.h"


//        CR1 Reg Bits Pos
#define PE_BIT_Pos       				0
#define NOSTRETCH_BIT_POS				7
#define START_BIT_Pos					8
#define STOP_BIT_Pos					9
#define ACK_BIT_POS						10


//        CR2 Reg Bits Pos
#define FREQ_BITS_Pos    				0
#define ITERREN_BIT_Pos    				8
#define ITEVTEN_BIT_Pos    				9
#define ITBUFEN_BIT_Pos    				10


//        SR1 Reg Bits Pos
#define SB_BIT_Pos		   				0
#define ADDR_BIT_Pos					1
#define BTF_BIT_Pos						2
#define ADD10_BIT_Pos					3
#define RxNE_BIT_Pos					6
#define TxE_BIT_Pos  					7


//        SR2 Reg Bits Pos
#define BUSY_BIT_Pos        			1


//        OAR1 Reg Bits Pos
#define ADD_10_BITS_POS     			0
#define ADD_1_7_BITS_POS    			1
#define ADD_MODE_BIT_POS 				15

//        OAR2 Reg Bits Pos
#define ENDUAL_BIT_POS      			0
#define ADD2_BITS_POS	    			1

//        CRR Reg Bits Pos
#define F_S_BIT_Pos      				15
#define CRR_BITS_Pos     				0

//        TRISE Reg Bits Pos
#define TRISE_BITS_Pos	 				0


#define	BUFFER_IRQ_EN  					8
#define EVENT_IRQ_EN					9
#define ERROR_IRQ_EN					10



#define RESERVED_FRAME_10BIT_ADDRESS	(0b11110 << 3)

// To Enable or Disable Feature
#define ENABLE							1U
#define DISABLE							0U



typedef enum
{
	WRITE,
	READ
}W_R;

typedef enum
{
	SM_MODE,
	FM_MODE
}I2C_Mode;


typedef enum
{
	_7BIT_ADD,
	_10BIT_ADD
}AddressMode;

typedef enum
{
	START,
	RepeatedSTART
}START_t;

typedef enum
{
	WithSTOP,
	WithoutSTOP
}STOP_t;

typedef struct
{
	START_t	START_Cond;


	STOP_t	STOP_Cond;
}SS_State;

typedef struct
{
	I2C_Periphral 	*I2Cx;

	I2C_Mode		SF_Mode;

	unsigned int 	SCL_Speed;

	unsigned char	SCL_Streach_EN;

	unsigned char	IRQ_EN;

	AddressMode		Add_Mode;

	unsigned int 	OwnAddress1;

	unsigned char	Dual_EN;

	unsigned int	OwnAddress2;



}I2C;



void I2C_INIT(I2C * hi2c);

void I2C_ActivateIRQ_Bit(I2C * hi2c, unsigned int IRQ_Activate);

void I2C_Master_Transmit(I2C * hi2c, unsigned int DevAddress, SS_State *Start_Stop,AddressMode Mode, unsigned char *payload, unsigned int lenght);

void I2C_Master_Recieve(I2C * hi2c, unsigned int DevAddress, SS_State *Start_Stop,AddressMode Mode, unsigned char *payload, unsigned int lenght);

void I2C_Slave_Transmit(I2C * hi2c);

void I2C_Slave_Recieve(I2C * hi2c);






#endif /* I2C_STM32F103C6_I2C_DRIVER_H_ */
