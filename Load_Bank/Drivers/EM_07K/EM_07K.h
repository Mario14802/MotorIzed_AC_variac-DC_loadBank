/*
 * EM_07K.h
 *
 *  Created on: Jun 18, 2025
 *      Author: Ramy Sorial
 */

#ifndef EM_07K_EM_07K_H_
#define EM_07K_EM_07K_H_

#include "../Modbus_RTU_C_Lib/Modbus_Master.h"

typedef enum EM_CMD{
	EM_Read_VT_CT = 4000,		    //2 words, CT/VT

	EM_Read_Phase_Voltages=4002, 	//12words Phase,Avg,Min,Max
	EM_Read_Input_Variac_V=4003,//	//12words Phase,Avg,Min,Max
	EM_Read_Line_Voltages=4014, 	//12 words line,avg,min,max

	EM_Read_Currents_H = 4026,		//15 words Curr,Avg,Min,Max,demand(Higher than 5A)
	EM_Read_Currents_L = 4027,		//15 words Curr,Avg,Min,Max,demand(Lower than 5A)

	EM_Read_P_Powers = 4042,   		//15 words Power,Avg,RSVD,Max,Demand
	EM_Read_S_Powers = 4072,  	    //15 words Power,Avg,RSVD,Max,Demand
	EM_Read_Freq = 4105,	        //12 words Freq,Avg,Min,Max
}EM_Cmd_t;

typedef struct EM_07K_t
{
	uint8_t Comm_OK :1; //1 for COMM ok
	uint8_t User_CMD_Cplt :1;
	uint8_t User_CMD_Success :1;

	uint8_t SLA;

	//address 4001,4002
	uint16_t VT_Ratio;
	uint16_t CT_Ratio;

	MB_Master_t *bus;


	int16_t Seq;
	EM_Cmd_t Current_CMD;
	uint16_t *p_data;
	uint8_t NumOfWords;
} EM_07K_t;

uint8_t EM_07_Init(EM_07K_t *EM);

uint8_t EM_07K_Read(EM_07K_t *EM, EM_Cmd_t Command, uint8_t NumOfWords,uint16_t *data);

void EM_07K_Routine(EM_07K_t *EM);

#endif /* EM_07K_EM_07K_H_ */
