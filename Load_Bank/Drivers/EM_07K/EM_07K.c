/*
 * EM_07K.c
 *
 *  Created on: Jun 18, 2025
 *      Author: Ramy Sorial
 */

#include "EM_07K.h"

uint8_t EM_07_Init(EM_07K_t *EM)
{
	EM->Seq = 0;
	EM->User_CMD_Cplt = 1;
	return EM_07K_Read(EM, EM_Read_VT_CT, 2, &EM->VT_Ratio);
}

uint8_t EM_07K_Read(EM_07K_t *EM, EM_Cmd_t Command, uint8_t NumOfWords, uint16_t *data)
{
	if (!EM->User_CMD_Cplt)
	{
		return 1;
	}
	else
	{
		EM->User_CMD_Cplt = 0;
		EM->User_CMD_Success = 0;
		EM->Current_CMD = Command;
		EM->NumOfWords = NumOfWords;
		EM->p_data = data;
		EM->Seq = 10;
		return 0;
	}
}

void EM_07K_Routine(EM_07K_t *EM)
{
	switch (EM->Seq)
	{
		//Idle state
		case 0:
			break;

			//Init state to read the VT and CT ratios
		case 10:
			if (!EM->bus->Busy)
			{
				MB_Master_ReadHoldingRegs(EM->bus, EM->SLA, EM->Current_CMD, EM->NumOfWords, EM->p_data);
				EM->Seq = 15;
			}
			break;

			//reply recieved
		case 15:
			if (!EM->bus->Busy)
			{
				switch (EM->bus->MB_Stat)
				{
					case MB_STAT_OK:
						EM->Comm_OK = 1;
						EM->User_CMD_Success = 1;
						EM->User_CMD_Cplt = 1;
						EM->Seq = 0;
						break;
					default:
						EM->Comm_OK = 0;
						EM->User_CMD_Success = 0;
						EM->User_CMD_Cplt = 1;
						EM->Seq = 0;
						break;
				}
				break;
			}
	}
}
