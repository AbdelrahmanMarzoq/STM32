/**
 ******************************************************************************
 * @File           : STM32F103C6_SPI_Driver.h
 * @Author         : Abdelrhman Marzoq
 * @Brief          : Driver Header file
 *
 ******************************************************************************
**/
#ifndef _SPI_DRIVER_H_
#define _SPI_DRIVER_H_


#include "../STM32F103X6.h"
#include "../GPIO/STM32F103C6_GPIO_Driver.h"
#include "../RCC/STM32F103C6_RCC_Driver.h"


// CR1 Bits Pos
#define CPHA_BIT_Pos 				0
#define CPOL_BIT_Pos 				1
#define MSTR_BIT_Pos  				2
#define BR_BITS_Pos					3 //[3-5]
#define SPE_BIT_Pos   				6
#define LSBFIRST_BIT_Pos			7
#define SSM_BIT_Pos					9
#define RXONLY_BIT_Pos				10
#define DFF_BIT_Pos					11
#define BIDIOE_BIT_Pos				14
#define BIDIMODE_BIT_Pos			15


// CR2 Bits Pos
#define TXEIE_BIT_Pos				7
#define RXNEIE_BIT_Pos				6
#define ERRIE_BIT_Pos				5

// SR Bits Pos
#define RXNE_BIT_Pos				0
#define TXE_BIT_Pos					1
#define BUSY_BIT_Pos 				7


/**********************************Reference Macros*************************************/
//@ref SPIx
// SPI1           for using SPI1
// SPI2           for using SPI2


//@ref SPI_Mode

typedef enum
{
	Half_Duplex,
	Full_Duplex
}Comm_Mode;


//@ref SPI_Master_Slave
// Bit 2 int CR1 Reg MSTR Identify Master Or Slave
typedef enum
{
	SPI_Slave,
	SPI_Master
}Device_Mode;

//@ref SPI_Frame_Size
typedef enum
{
	SPI_BIT_8,
	SPI_BIT_16
}Data_Size;

//@ref SPI_SCLK
typedef enum
{
	SPI_BAURDRATEPRESCALLER_2   = 0    ,
	SPI_BAURDRATEPRESCALLER_4   = 0b001,
	SPI_BAURDRATEPRESCALLER_8   = 0b010,
	SPI_BAURDRATEPRESCALLER_16  = 0b011,
	SPI_BAURDRATEPRESCALLER_32  = 0b100,
	SPI_BAURDRATEPRESCALLER_64  = 0b101,
	SPI_BAURDRATEPRESCALLER_128 = 0b110,
	SPI_BAURDRATEPRESCALLER_256 = 0b111
}BaudRate_Prescaller;



//@ref SPI_IRQ
typedef enum
{
	SPI_IRQ_NONE,      							// Disable IRQ
	SPI_IRQ_TXEIE 	= 	1 << TXEIE_BIT_Pos,		// Enable IRQ When Transmitt Buffer empty
	SPI_IRQ_RXNEIE 	=  	1 << RXNEIE_BIT_Pos,	// Enable IRQ When Reciever Buffer not Empty
	SPI_IRQ_ERRIE  	=	1 << ERRIE_BIT_Pos		// Enable Interrupt IRQ

}IRQ_EN;

/***************************************************************************************/


typedef struct
{
	SPI_Periphral        *SPIx				;			// Specifies Which SPI Use.
														// This parameter must be set based on @ref SPIx

	Comm_Mode			 Communiaction_Mode ;			// Specifies Which SPI Mode.
														// This parameter must be set based on @ref SPI_Mode

	Device_Mode			 Master_Slave_Mode	;			// Specifies Master || Slave.
														// This parameter must be set based on @ref SPI_Master_Slave

	Data_Size			 Frame_Size			;			// Specifies 8BIT Or 16BIT Frame.
														// This parameter must be set based on @ref SPI_Frame_Size

	BaudRate_Prescaller	 Prescaller			;			// Specifies SCLK Freq.
														// This parameter must be set based on @ref SPI_SCLK



	IRQ_EN				 IRQ				;			// Specifies IRQ En Or Dis.
														// This parameter must be set based on @ref SPI_IRQ

	void(*IRQ_CALL)()						;			// This Function Will Call if you Enable IRQ Of Periphral

}SPI;



/*
 * =======================================================================================
 * 										APIs
 * =======================================================================================
*/



/**================================================================
 * @Fn                 	- SPI_MASTER_INIT
 * @brief				- You Must Call This Func Before Any Using APIS
 * @param [in]			- Pointer to your SPI
 *
 * @retval				- NONE
 * Note					- NONE
*/
void SPI_MASTER_INIT(SPI *SPI_Conf);

/**================================================================
 * @Fn                 	- SPI_RX
 * @brief				- To Recieve From RX Buffer
 * @param [in]			- Pointer to your SPI and Your Buffer to reciev
 *
 * @retval				- NONE
 * Note					- NONE
*/
void SPI_RX(SPI *SPI_Conf, uint16_t *RX_Buffer);

/**================================================================
 * @Fn                 	- SPI_TX
 * @brief				- To Transmit Your TX Buffer
 * @param [in]			- Pointer to your SPI and Your Buffer that Need to Transmit
 *
 * @retval				- NONE
 * Note					- NONE
*/
void SPI_TX(SPI *SPI_Conf, uint16_t *TX_Buffer);

/**================================================================
 * @Fn                 	- SPI_RX_TX
 * @brief				- To Transmit and recieve at the same time
 * @param [in]			- Pointer to your SPI and Your Buffer that Need to Transmit and Store in it The RX Buffer
 *
 * @retval				- NONE
 * Note					- NONE
*/
void SPI_RX_TX(SPI *SPI_Conf, uint16_t *TX_RX_Buffer);

/**================================================================
 * @Fn                 	- SPI_deINIT
 * @brief				- Disable Periphral
 * @param [in]			- Pointer to your SPI
 *
 * @retval				- NONE
 * Note					- NONE
*/
void SPI_deINIT(SPI *SPI_Conf);


#endif

