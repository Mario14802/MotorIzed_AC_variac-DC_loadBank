/*
 * NVM_Variables.h
 *
 *  Created on: Sep 25, 2024
 *      Author: Ramy Sorial
 */

#ifndef INC_NVM_VARIABLES_H_
#define INC_NVM_VARIABLES_H_

#include "TypesAndRegMap.h"
#include "../../Drivers/EEPROM_FLASH/EEPROM_Helper.h"
#include <stdint.h>
#include <string.h>

/**
 * @fn uint8_t Load_NV_Params()
 * @brief
 *	loads the NV data from the flash to the RAM
 * @return 1 if the checksum was successful
 */
uint8_t Load_NV_Params();

/**
 * @fn void Save_NV_Params()
 * @brief
 *	saves the parameters to the flash
 */
void Save_NV_Params();

/**
 * @fn void Load_Default_NV_Params()
 * @brief
 * Loads the default params to the NVROM
 */
void Load_Default_NV_Params();

#endif /* INC_NVM_VARIABLES_H_ */
