/*
 * EEPROM.c
 *
 *  Created on: Oct 17, 2024
 *      Author: Abdelrhman Marzoq
 */


#include "EEPROM.h"

I2C hi2c1;
void EEPROM_init()
{

	hi2c1.I2Cx = I2C1;
	hi2c1.Add_Mode = _7BIT_ADD;
	hi2c1.SCL_Speed = 100000;
	hi2c1.Dual_EN= DISABLE;
	hi2c1.IRQ_EN= DISABLE;
	hi2c1.SCL_Streach_EN= ENABLE;
	hi2c1.SF_Mode= DISABLE;

	I2C_INIT(&hi2c1);
}


void EEPROM_Write(uint16_t Memory_Address, uint8_t *DataBuffer, uint8_t Length)
{
	SS_State Start_Stop;
	Start_Stop.START_Cond = START;
	Start_Stop.STOP_Cond = WithSTOP;
	uint8_t Buffer [256];
	Buffer[0] = (unsigned char)((Memory_Address & 0xFF00) >> 8); 	// Upper Byte of address
	Buffer[1] = (unsigned char)(Memory_Address & 0x00FF); 			// Lower Byte of address

	for (uint16_t i = 2; i < Length+2; i++)
	{
		Buffer[i] = *DataBuffer;
		DataBuffer++;
	}

//	Write on the Mem Address Data
	I2C_Master_Transmit(&hi2c1, EEPROM_Slave_Address, &Start_Stop, _7BIT_ADD, Buffer, (Length+2));

//	I2C_Master_Write(&I2C_Instant, Bit_7, EEPROM_Slave_Address, bytes, Length+2, With_Stop,Start);
}

void EEPROM_Read(uint16_t Memory_Address, uint8_t *DataBuffer, uint8_t Length)
{
	SS_State Start_Stop;
	Start_Stop.START_Cond = START;
	Start_Stop.STOP_Cond = WithoutSTOP;

	uint8_t Buffer[2];
	Buffer[0] = (unsigned char)((Memory_Address & 0xFF00) >> 8); 	// Upper Byte of address
	Buffer[1] = (unsigned char)(Memory_Address & 0x00FF); 			// Lower Byte of address



	I2C_Master_Transmit(&hi2c1, EEPROM_Slave_Address, &Start_Stop, _7BIT_ADD, Buffer, 2);

	Start_Stop.START_Cond = RepeatedSTART;
	Start_Stop.STOP_Cond = WithSTOP;

	I2C_Master_Recieve(&hi2c1, EEPROM_Slave_Address, &Start_Stop, _7BIT_ADD, DataBuffer, 2);

}
