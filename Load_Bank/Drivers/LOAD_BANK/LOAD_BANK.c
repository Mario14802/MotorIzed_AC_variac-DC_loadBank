/*
 * LOAD_BANK.c
 *
 *  Created on: Jul 2, 2025
 *      Author: MARIO
 */


#include "LOAD_BANK.h"




void LOAD_BANK_Init(LOAD_BANK_t *LB)
{
	LB->Seq = 0;
	LB->User_CMD_Cplt = 1;

}

uint8_t LOAD_BANK_Read(LOAD_BANK_t *LB, LOAD_BANK_CMD_t Command, uint8_t NumOfWords, uint16_t *data,REG_Type_t Type)
{
	if (!LB->User_CMD_Cplt)
	{
		return 1;
	}
	else
	{
		LB->User_CMD_Cplt = 0;
		LB->User_CMD_Success = 0;
		LB->Current_CMD = Command;
		LB->NumOfWords = NumOfWords;
		LB->p_data = data;

		//if the type to be read is Input registers
		if(Type==INPUT_REG)
		{
			LB->Seq = 10;
		}
		//if the type to be read is holding registers
		else if (Type==HOLDING_REG_Read)
		{
			LB->Seq=15;
		}


		return 0;
	}
}



uint8_t LOAD_BANK_Write(LOAD_BANK_t *LB, LOAD_BANK_CMD_t Command, uint16_t data)
{


	if (!LB->User_CMD_Cplt)
	{
		return 1;
	}
	else
	{
		LB->User_CMD_Cplt = 0;
		LB->User_CMD_Success = 0;
		LB->Current_CMD = Command;
		LB->Data = data;
		LB->Seq=20;

		return 0;
	}




}




void LOAD_BANK_Routine(LOAD_BANK_t *LB)
{
	switch (LB->Seq)
	{

	//Idle state
	case 0:
		break;

		////////////////// To Read Input Registers /////////////////////
	case 10:
		if (!LB->bus->Busy)
		{
			MB_Master_ReadInputRegs(LB->bus, LB->SLA, LB->Current_CMD, LB->NumOfWords, LB->p_data);
			LB->Seq = 25;
		}
		break;

		///////////////To Read Holding Registers ////////////////////
	case 15:
		if (!LB->bus->Busy)
		{
			MB_Master_ReadHoldingRegs(LB->bus, LB->SLA, LB->Current_CMD, LB->NumOfWords, LB->p_data);
			LB->Seq = 25;
		}
		break;


	case 20:
		if (!LB->bus->Busy)
		{
			MB_Master_WriteHoldingReg(LB->bus, LB->SLA, LB->Current_CMD, LB->Data);
			LB->Seq = 25;
		}
		break;

		//reply recieved
	case 25:
		if (!LB->bus->Busy)
		{
			switch (LB->bus->MB_Stat)
			{
			case MB_STAT_OK:
				LB->Comm_OK = 1;
				LB->User_CMD_Success = 1;
				LB->User_CMD_Cplt = 1;
				LB->Seq = 0;
				break;
			default:
				LB->Comm_OK = 0;
				LB->User_CMD_Success = 0;
				LB->User_CMD_Cplt = 1;
				LB->Seq = 0;
				break;
			}
			break;
		}
	}
}
