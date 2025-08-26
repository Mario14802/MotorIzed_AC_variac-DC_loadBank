/*
 * HW_Interface.c
 *
 *  Created on: May 11, 2025
 *      Author: Ramy Sorial
 */

#include "HW_Interface.h"

uint16_t HoldingRegs[HoldingRegsSize];
uint16_t InputRegs[InputRegsSize];

uint8_t TxBuffer_S[256];
uint8_t RxBuffer_S[256];

uint8_t CoilBits[4];
uint8_t InputBits[4];

HoldingRegMap *Hregs;
InputRegMap *Iregs;

//modbus port over USB
MB_Slave_t MB =
{ .SLA =10,
		.TX_RX_Buffer_Size = sizeof(TxBuffer_S),
		.TxBuffer = TxBuffer_S,
		.RxBuffer = RxBuffer_S,
		.HoldingRegs = HoldingRegs,
		.InputRegs = InputRegs,
		.InputBits = InputBits,
		.CoilBits = CoilBits,
		.CoilBitsSize = sizeof(CoilBits),
		.InputBitsSize = sizeof(InputBits),
		.HoldingRegSize = HoldingRegsSize,
		.InputRegSize = InputRegsSize,
		.TX_Automplete = 0,
		.RX_Timeout = 100,
		.RX_Silent_Interval_MS = 2 };

MB_Master_t MS;

uint8_t TxBuffer_M[256];
uint8_t RxBuffer_M[256];

MB_Master_t MS = {
		.TX_Automplete = 0,
		.TxBuffer = TxBuffer_M,
		.RxBuffer = RxBuffer_M,
		.RX_Timeout = 200,
		.TX_RX_BufferSize = sizeof(TxBuffer_M),
		.hw_interface = {
				.MB_Transmit = MS_Transmit,
				.MB_StartListening = MS_StartListening,
				.MB_Activate_TX = MS_Activate_TX,
				.MB_Request_Recieved =MS_Request_Recieved
		},
};

EM_07K_t Multimeter = {
		.SLA = 2,
		.bus = &MS,
};

LOAD_BANK_t LB = {
		//new master addresse could be changed  by sending the id you want to add 0x0000
		.SLA = 1,
		.bus = &MS,
};


ADS11xx_t ads =
{
		.Chip_Address = ADS11xx_AD0_GND,

		.FSR = ADS11xx_FSR_4_096,

		.MuxConfig= ADS11xx_MUX_AIN0_GND,

		.SamplingRate=ADS11xx_64SPS,

		/// @brief set to 1 for single shot conversion mode
		.Single_Shot =0,

		/// @brief set to 1 for Window mode, 0 for Traditional mode
		.Comp_Mode =0,

		/// @brief sets the comparator polarity (1 for active high)
		.Comp_Pol =0,

		/// @brief set to 1 for latching mode
		.Comp_Lat =0,

		/// @brief set to to trigger a conversion
		.OS=0,

		/// @brief sets the queue mode:
		/// 00 : Assert after 1 conversion
		/// 01 : Assert after 2 conversions
		/// 10 : Assert after 4 conversions
		///	11 : Comparator Disabled
		.Comp_Que =3,

		/// @brief Comparator low threshold register
		//.Comp_L_Thresh=0,
		/// @brief Comparator high threshold register
		//.Comp_H_Thresh=1,

		.HW_Interface = {
				.I2C_Write=I2C_Write,
				.I2C_Read=I2C_Read,
		}


};

uint8_t i2c_done=0;

uint8_t I2C_Write(uint8_t SLA,  uint8_t *data, uint8_t len)
{
	HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)(SLA<<1), data, (uint16_t)len, 200);
	//HAL_I2C_Master_Transmit_IT(&hi2c1, (uint16_t)(SLA<<1), data, (uint16_t)len);
	return ADS11xx_OK;
}
uint8_t I2C_Read(uint8_t SLA,  uint8_t *data, uint8_t len)
{
	HAL_I2C_Master_Receive(&hi2c1, (uint16_t)(SLA<<1), data, (uint16_t)len, 200);
	//HAL_I2C_Master_Receive_IT(&hi2c1, (uint16_t)(SLA<<1), data, (uint16_t)len);
	return ADS11xx_OK;
}

MB_Status_t MB_Transmit(uint8_t *data, uint16_t len)
{
	HAL_UART_Transmit_DMA(&huart1, data, len);
	return MB_STAT_OK;
}
MB_Status_t MB_StartListening()
{
	return HAL_UARTEx_ReceiveToIdle_DMA(&huart1, MB.RxBuffer, MB.TX_RX_Buffer_Size) == HAL_OK ? MB_STAT_OK : MB_TX_ERR_HW;
}
MB_Status_t MB_Activate_TX(uint8_t TX_ON)
{

	return MB_STAT_OK;
}

//scanhead master Modbus functions
MB_Status_t MS_Transmit(uint8_t *data, uint16_t len)
{
	HAL_UART_Transmit_DMA(&huart3, data, len);
	return MB_STAT_OK;
}
MB_Status_t MS_StartListening()
{
	return HAL_UARTEx_ReceiveToIdle_DMA(&huart3, MS.RxBuffer, MS.TX_RX_BufferSize) == HAL_OK ? MB_STAT_OK : MB_TX_ERR_HW;
}
MB_Status_t MS_Activate_TX(uint8_t TX_ON)
{
	HAL_GPIO_WritePin(RS_R_EN_GPIO_Port, RS_R_EN_Pin, TX_ON);
	HAL_GPIO_WritePin(RS_W_EN_GPIO_Port, RS_W_EN_Pin, TX_ON);
	return MB_STAT_OK;
}

//HAL callback functions
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == huart1.Instance)
	{
		MB.RX_LEN = Size;
	}
	else if (huart->Instance == huart3.Instance)
	{
		MS.RX_LEN = Size;
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == huart1.Instance)
	{
		MB_Slave_TX_Complete(&MB);
	}
	else if (huart->Instance == huart3.Instance)
	{
		MB_Master_TX_Complete(&MS);
	}
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	 if (hi2c->Instance == hi2c1.Instance) i2c_done = true;
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	 if (hi2c->Instance == hi2c1.Instance) i2c_done = true;
}

void Program_Reset()
{
	HAL_NVIC_SystemReset();
}

void System_Init()
{
	MB.hw_interface.MB_Activate_TX = MB_Activate_TX;
	MB.hw_interface.MB_StartListening = MB_StartListening;
	MB.hw_interface.MB_Transmit = MB_Transmit;
	MB.hw_interface.MB_Request_Recieved = MB_Request_Recieved;
	MB_Slave_Init(&MB);

	//link the data structures to their actual memory locations
	Hregs = (HoldingRegMap*) MB.HoldingRegs;
	Iregs = (InputRegMap*) MB.InputRegs;

	MB_Master_Init(&MS);

	//rsgister the UARt callbacks for the modbus
	HAL_UART_RegisterRxEventCallback(&huart1, HAL_UARTEx_RxEventCallback);
	HAL_UART_RegisterRxEventCallback(&huart3, HAL_UARTEx_RxEventCallback);

	HAL_UART_RegisterCallback(&huart1, HAL_UART_TX_COMPLETE_CB_ID, HAL_UART_TxCpltCallback);
	HAL_UART_RegisterCallback(&huart3, HAL_UART_TX_COMPLETE_CB_ID, HAL_UART_TxCpltCallback);

	//load the NV data from memory
	if (!Load_NV_Params())
	{
		Load_Default_NV_Params();
		//Save_NV_Params();
	}
}

