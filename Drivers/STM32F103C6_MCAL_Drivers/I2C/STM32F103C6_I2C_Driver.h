/**
 ******************************************************************************
 * @File           : STM32F103C6_I2C_Driver.h
 * @Author         : Abdelrhman Marzoq
 * @Brief          : Driver Header File
 * 			Created on: Oct 10, 2024
 ******************************************************************************
**/

#ifndef _I2C_DRIVER_H_
#define _I2C_DRIVER_H_


#include "../STM32F103X6.h"
#include "../GPIO/STM32F103C6_GPIO_Driver.h"
#include "../RCC/STM32F103C6_RCC_Driver.h"




#define Writing_Operation   1
#define MASTER_BYTE_TRANSMIT 0x00070080



//        CR1 Reg Bits Pos
#define PE_BIT_Pos       	0
#define START_BIT_Pos		8
#define STOP_BIT_Pos		9
#define ACK_BIT_POS			10

//        CR2 Reg Bits Pos
#define FREQ_BITS_Pos    	0

//        CRR Reg Bits Pos
#define F_S_BIT_Pos      	15
#define CRR_BITS_Pos     	0

//        TRISE Reg Bits Pos
#define TRISE_BITS_Pos	 	0

//        OAR1 Reg Bits Pos
#define ADD_10_BITS_POS     0
#define ADD_1_7_BITS_POS    1
#define ADD_MODE_BIT_POS 	15

//        OAR2 Reg Bits Pos
#define ENDUAL_BIT_POS      0
#define ADD2_BITS_POS	    1

//        SR1 Reg Bits Pos
#define SB_BIT_Pos		    0
#define ADDR_BIT_Pos		1
#define BTF_BIT_Pos			3
#define RxNE_BIT_Pos		6
#define TxE_BIT_Pos  		7


//        SR2 Reg Bits Pos
#define BUSY_BIT_Pos        1







/**********************************  References  *************************************/

// @ref I2Cx
// I2C1					For using I2C1
// I2C2					For using I2C2


// @ref I2C_Mode
typedef enum
{
	SM_Mode,                    // I2C Standard Mode Clk Up to 100KHz   (Recommended)
	FM_Mode						// I2C Fast Mode CLK up to 400KHz		(Not Supported in Driver)
}I2C_Mode;

// @ref SCL_Speed
typedef enum
{
	SCL_50KHz 	= 50000,                    	// I2C SCL Bus Speed 50KHz							For SM Mode
	SCL_100KHz 	= 100000,						// I2C SCL Bus Speed 100KHz			(Recommended)	For SM Mode
	/******************************* FM Mode not supported in driver *******************************/
	SCL_200KHz 	= 200000,						// I2C SCL Bus Speed 200KHz							For FM Mode
	SCL_400KHz 	= 400000						// I2C SCL Bus Speed 400KHz							For FM Mode
}SCL_Speed;


// @ref Address_Mode
typedef enum
{
	Bit_7,
	Bit_10
}Address_Mode;

// @ref Dual_EN
typedef enum
{
	DISABLE,
	ENABLE
}Dual_EN;


typedef enum
{
	Start,
	Repeated
}Start_Cond;

typedef enum
{
	Without_Stop,
	With_Stop
}Stop_Cond;


/***********************************************************************/


/* Implement it if you need MicroController Act as Slave not Master */
typedef struct
{
	 Address_Mode Mode;				// Choosing Address Mode based on @ref Address_Mode
	 Dual_EN Dual;					// Enable & Disable Dual Address based on @ref Dual_EN
	 uint16_t Address1;				// Specify First Address
	 uint16_t Address2;				// Specify Second Address if Dual is Enabled
}Slave;

typedef struct
{
	I2C_Periphral *I2Cx;					// Specifies Which I2C Use.
											// This parameter must be set based on @ref I2Cx

	I2C_Mode	   Mode;					// Specifies Which I2C Mode.
											// This parameter must be set based on @ref I2C_Mode

	SCL_Speed	   SCL_Speed;				// Specifies SCL Bus Speed.
											// This parameter must be set based on @ref SCL_Speed

}I2C;




/*
 * =======================================================================================
 * 										APIs
 * =======================================================================================
*/

/**================================================================
 * @Fn                 	- I2C_INIT
 * @brief				- Init I2C Periphral
 *  					  Must Call First Befor any Sending or Recieving Data
 * @param [in]			- Pointer to I2C
 *
 * @retval				- NONE
 * Note					- NONE
 *================================================================*/
void I2C_INIT(I2C *I2C_Conf);

/**================================================================
 * @Fn                 	- I2C_Slave_Init
 * @brief				- Init MCU as Slave
 * @param [in]			- Pointer to I2C , Pointer to struct Include Address to act as Slave
 *
 * @retval				- NONE
 * Note					- NONE
 *================================================================*/
void I2C_Slave_Init (I2C *I2C_Conf, Slave *Address);


/**================================================================
 * @Fn                 	- I2C_Master_Write
 * @brief				-
 * @param [in]			-
 *
 * @retval				- NONE
 * Note					- NONE
 *================================================================*/
void I2C_Master_Write(I2C *I2C_Conf, Address_Mode Mode, uint16_t devAddr, uint8_t *data, uint32_t dataLength, Stop_Cond Stop,Start_Cond Start);

/**================================================================
 * @Fn                 	- I2C_Master_Read
 * @brief				-
 * @param [in]			-
 *
 * @retval				- NONE
 * Note					- NONE
 *================================================================*/
void I2C_Master_Read(I2C *I2C_Conf, Address_Mode Mode, uint16_t devAddr, uint8_t *dataOut, uint32_t dataLength, Stop_Cond Stop, Start_Cond Start);

/**================================================================
 * @Fn                 	- I2C_deINIT
 * @brief				- De Init I2C Bus (Free Pins)
 * @param [in]			- Pointer to I2C
 *
 * @retval				- NONE
 * Note					- NONE
 *================================================================*/
void I2C_deINIT(I2C *I2C_Conf);


#endif
