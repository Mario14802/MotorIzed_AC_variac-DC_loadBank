/*
 * NVM_Params.c
 *
 *  Created on: May 24, 2025
 *      Author: Ramy Sorial
 */

#include "NVM_Variables.h"

/// where the default params are stored
const MachineParams DefaultParams =
{ };



/**
 * @fn uint16_t Compute_Checksum(uint8_t*, uint16_t)
 * @brief
 *	computes the memory checksum
 * @param data ptr to the data to have the checksum computed for
 * @param Len number of data bytes
 * @return
 */
static uint16_t Compute_Checksum(uint8_t *data, uint16_t Len)
{
	uint16_t Checksum = 0;
	for (uint16_t x = 0; x != Len; x++)
	{
		Checksum += data[x];
	}
	return Checksum;
}

uint8_t Load_NV_Params()
{
	EEPROM_Process_Area(0, (uint8_t*) &Hregs->Params, sizeof(Hregs->Params), 1);
	if (Compute_Checksum((uint8_t*) &Hregs->Params, sizeof(Hregs->Params) - 2) == Hregs->Params.Checksum)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void Save_NV_Params()
{
	Hregs->Params.Checksum = Compute_Checksum((uint8_t*) &Hregs->Params, sizeof(MachineParams) - 2);
	EEPROM_Process_Area(0, (uint8_t*) &Hregs->Params, sizeof(Hregs->Params), 0);
}

void Load_Default_NV_Params()
{
	memcpy(&Hregs->Params, &DefaultParams, sizeof(MachineParams));
	Hregs->Params.Checksum = Compute_Checksum((uint8_t*) &Hregs->Params, sizeof(MachineParams) - 2);
}

