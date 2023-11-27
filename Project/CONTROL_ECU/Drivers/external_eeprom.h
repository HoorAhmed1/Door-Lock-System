/*
 * external_eeprom.h
 *
 *  Created on: Nov 8, 2023
 *      Author: Dell
 */

#ifndef DRIVERS_EXTERNAL_EEPROM_H_
#define DRIVERS_EXTERNAL_EEPROM_H_

#include "std_types.h"

#define SUCCESS 1
#define ERROR 0

uint8 EEPROM_writeByte(uint16 address,uint8 data);
uint8 EEPROM_readByte(uint16 address,uint8 *data);


#endif /* DRIVERS_EXTERNAL_EEPROM_H_ */
