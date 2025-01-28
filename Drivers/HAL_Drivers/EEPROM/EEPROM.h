/*
 * EEPROM.h
 *
 *  Created on: Oct 17, 2024
 *      Author: Abdelrhman Marzoq
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "../STM32F103C6_MCAL_Drivers/I2C/STM32F103C6_I2C_Driver.h"



#define EEPROM_Slave_Address 0x2A



void EEPROM_init();

void EEPROM_Write(uint16_t Memory_Address, uint8_t *DataBuffer, uint8_t Length);

void EEPROM_Read(uint16_t Memory_Address, uint8_t *DataBuffer, uint8_t Length);



#endif /* EEPROM_EEPROM_H_ */
