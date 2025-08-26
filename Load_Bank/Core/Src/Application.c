/*
 * Application.c
 *
 *  Created on: May 14, 2025
 *      Author: Ramy Sorial
 */

#include "Application.h"
PI_Handle_t PI;

float KP=0.2;

float SP;
float PV;
float Out;
uint32_t Time;
uint16_t AC_Current;
///ADS11 ADC READING AND STATUS
int16_t ADC_Value;
uint8_t ADC_Status;
uint8_t ADCConf_Status;
volatile uint8_t ADC_FLAG;

MB_Status_t MB_Request_Recieved(void *ptr)
{
	return MB_STAT_OK;
}

MB_Status_t MS_Request_Recieved(void *ptr)
{
	return MB_STAT_OK;
}

//ProtoType
void System_Routine();
void compute_analog_measurement();
void Motor_Switch();

void Application_Init()
{
	System_Init();
	HAL_TIM_Base_Start_IT(&htim3);

	MB_StartListening();
	MS_StartListening();

	EM_07_Init(&Multimeter);
	LOAD_BANK_Init(&LB);
	PI_Init(&PI, KP);

	ADS11xx_Init(&ads);
}

inline void Application_Run()
{
	static uint16_t last_adc_tick;
	//static uint16_t AC_Seq;
	static uint32_t M_Ticks;
	//static uint32_t AC_Ticks;
	//uint32_t now = HAL_GetTick();

	while (1)
	{
		//modbus master/slave routine
		MB_Slave_Routine(&MB, HAL_GetTick());
		MB_Master_Routine(&MS, HAL_GetTick());

		//multimeter routine
		EM_07K_Routine(&Multimeter);
		LOAD_BANK_Routine(&LB);

		if (HAL_GetTick() >= M_Ticks)
		{
			if (MS.Busy!=1)
			{

				_MB_SetInput(LOAD_BANK_COMM_OK, LB.Comm_OK);
				_MB_SetInput(Multimeter_COMM_OK, Multimeter.Comm_OK);
				System_Routine();
			}
			/////////LED AS an indication ////////////////////////
			if (LB.Comm_OK)
			{
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
			}


			M_Ticks = HAL_GetTick() + 50;
		}

		///////////////MOTOR Switch states///////
		//void Motor_Switch();


		Variac_Test(RELAY_0_GPIO_Port, RELAY_0_Pin, ENABLE);
		Variac_Test(RELAY_1_GPIO_Port, RELAY_1_Pin,ENABLE );
		Variac_Test(RELAY_2_GPIO_Port, RELAY_2_Pin,ENABLE );
		Variac_Test(RELAY_3_GPIO_Port, RELAY_3_Pin, ENABLE);
		Variac_Test(RELAY_4_GPIO_Port, RELAY_4_Pin, ENABLE);
		Variac_Test(RELAY_5_GPIO_Port, RELAY_5_Pin, ENABLE);
		Variac_Test(RELAY_6_GPIO_Port, RELAY_6_Pin,ENABLE );
		Variac_Test(RELAY_7_GPIO_Port, RELAY_7_Pin, ENABLE);

		/*// TO READ AC_VOLTAGE BELOW 5V
		if (HAL_GetTick() - last_adc_tick >= 200)
		{
			ADS11xx_Read(&ads, &ADC_Value, &ADC_Status);
			compute_analog_measurement();
			ADC_FLAG=0;

		}


		if(Hregs->Params.AC_Voltage_SP <= Threshold)
		{
			Iregs->AC_Voltage=Iregs->AC_V_BELOW_5V;
			VOLTAGE_AC_S(ENABLE);

		}
		else
		{
			Iregs->AC_Voltage=Iregs->AC_Voltage_H;
			VOLTAGE_AC_S(DISABLE);
		}

		if(Hregs->Params.AC_Current_SP <= Threshold)//Threshold = 5
		{
			Iregs->AC_Current=Iregs->AC_Current_L;
			Current_AC_S(ENABLE);
		}
		else
		{
			Iregs->AC_Current=Iregs->AC_Current_H;
			Current_AC_S(DISABLE);
		}



		 */


	}
}
void System_Routine()
{
	static uint16_t M_Seq;

	switch (M_Seq)
	{
	case 0:

		LOAD_BANK_Read(&LB, HREG_SLAVE_ADD, 13, &Hregs->Params.Read_Slave_address, HOLDING_REG_Read);
		M_Seq = 10;
		break;

	case 10:

		LOAD_BANK_Read(&LB, IREG_SLAVE_ADD, 13, &Iregs->Slave_address, INPUT_REG);
		M_Seq = 20;
		break;

	case 20:

		LOAD_BANK_Write(&LB, HREG_MEASUREMENT_OPERATING_MODE,  Hregs->Params.Operating_mode);
		M_Seq = 30;
		break;

	case 30:
		LOAD_BANK_Write(&LB, HREG_MEASUREMENT_TEST_ENABLE, Hregs->Params.Test_Enable_Disable);
		M_Seq = 40;
		break;

	case 40:
		LOAD_BANK_Write(&LB, HREG_SET_VOLTAGE_VALUE, Hregs->Params.CV_set_value);
		M_Seq = 50;
		break;

	case 50:
		LOAD_BANK_Write(&LB, HREG_SET_CURRENT,  Hregs->Params.CI_set_value);
		M_Seq = 60;
		break;

	case 60:
		LOAD_BANK_Write(&LB, HREG_SET_CONSTANT_POWER_VALUE, Hregs->Params.CP_set_value);
		M_Seq = 70;
		break;

	case 70:
		LOAD_BANK_Write(&LB, HREG_SET_RESISTANCE_VALUE, Hregs->Params.CR_set_value);
		M_Seq = 80;
		break;

	case 80:
		LOAD_BANK_Write(&LB, HREG_OPERATE_OPTIONS, Hregs->Params.Options);
		M_Seq = 90;
		break;

		//MultiMeter Readings
		//READ PHASE_1 VOLTAGE
	case 90:
		EM_07K_Read(&Multimeter, EM_Read_Phase_Voltages, 1, &Iregs->AC_Voltage_H);
		M_Seq = 100;
		break;
		//READ HIGH CURRENT >5
	case 100:
		EM_07K_Read(&Multimeter, EM_Read_Currents_H, 1, &Iregs->AC_Current_H);
		M_Seq = 110;
		break;
		//READ LOW CURRENT <5
	case 110:
		EM_07K_Read(&Multimeter, EM_Read_Currents_L, 1, &Iregs->AC_Current_L);
		M_Seq = 120;
		break;
		// READ INPUT_VOLTAGE
	case 120:
		EM_07K_Read(&Multimeter, EM_Read_Input_Variac_V, 1, &Iregs->AC_Input_Voltage);
		M_Seq = 130;
		break;

	case 130:
		EM_07K_Read(&Multimeter, EM_Read_Freq, 1, &Iregs->AC_Freq);
		M_Seq = 0;
		break;


	}
}

void Relay_test()
{

	Variac_Test(RELAY_0_GPIO_Port, RELAY_0_Pin, ENABLE);
	Variac_Test(RELAY_1_GPIO_Port, RELAY_1_Pin,ENABLE );
	Variac_Test(RELAY_2_GPIO_Port, RELAY_2_Pin,ENABLE );
	Variac_Test(RELAY_3_GPIO_Port, RELAY_3_Pin, ENABLE);
	Variac_Test(RELAY_4_GPIO_Port, RELAY_4_Pin, ENABLE);
	Variac_Test(RELAY_5_GPIO_Port, RELAY_5_Pin, ENABLE);
	Variac_Test(RELAY_6_GPIO_Port, RELAY_6_Pin,ENABLE );
	Variac_Test(RELAY_7_GPIO_Port, RELAY_7_Pin, ENABLE);

}
void compute_analog_measurement()
{
	//using fixed point
	//shift left (1<<15) insited of division for better code optimization as division is taxing on the processor
	Iregs->AC_V_BELOW_5V = (int16_t)( ((uint32_t)ADC_Value*(uint32_t)4096)/((uint32_t)(1<<15)));


}

void Motor_Switch()
{
	///////////////MOTOR Switch states///////
	static uint16_t AC_Seq;
	static uint32_t AC_Ticks;
	uint32_t now = HAL_GetTick();


	switch (AC_Seq)
	{
	case AC_STATE_CALC:

		SP= (float)Hregs->Params.AC_Current_SP;
		PV= (float)AC_Current;

		Out=PI_Eval(&PI, SP, PV);
		uint32_t delay;
		if(Out>0)
		{
			Variac_CW(ENABLE);
			Variac_CCW(DISABLE);
			delay = (uint32_t)Out;


		}
		else if (Out<0)
		{


			Variac_CCW(ENABLE);
			Variac_CW(DISABLE);
			delay = (uint32_t)(-Out);
		}

		else
		{
			Variac_CW(DISABLE);
			Variac_CCW(DISABLE);
			delay = 0;
		}


		AC_Ticks = now + delay;
		AC_Seq=AC_STATE_OFF;

		break;

	case AC_STATE_OFF :

		if (HAL_GetTick() >= AC_Ticks)
		{
			AC_Seq=AC_STATE_CALC;
		}
		break;
	}
}
