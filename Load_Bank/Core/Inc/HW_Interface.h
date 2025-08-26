/*
 * HW_Interface.h
 *
 *  Created on: May 11, 2025
 *      Author: Ramy Sorial
 */

#ifndef INC_HW_INTERFACE_H_
#define INC_HW_INTERFACE_H_

#include "main.h"
#include "stm32f1xx_hal.h"
#include "TypesAndRegMap.h"
#include "NVM_Variables.h"
#include "../../Drivers/Modbus_RTU_C_Lib/Modbus_Master.h"
#include "../../Drivers/Modbus_RTU_C_Lib/Modbus_Slave.h"
#include "../../Drivers/EM_07K/EM_07K.h"
#include "../../Drivers/LOAD_BANK/LOAD_BANK.h"
#include "../../Drivers/ADS11/ADS11xx.h"


extern I2C_HandleTypeDef hi2c1;
extern ADS11xx_t ads;

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern MB_Slave_t MB;
extern MB_Master_t MS;

extern EM_07K_t Multimeter;
extern LOAD_BANK_t LB;

//to be mapped to the actual holding registers map
extern HoldingRegMap *Hregs;

//to be mapped to the actual input registers map
extern InputRegMap *Iregs;

uint8_t I2C_Write(uint8_t SLA, uint8_t *Data, uint8_t len);
uint8_t I2C_Read(uint8_t SLA, uint8_t *Data, uint8_t len);


MB_Status_t MB_Request_Recieved(void *ptr);
MB_Status_t MB_Transmit(uint8_t *data, uint16_t len);
MB_Status_t MB_StartListening();
MB_Status_t MB_Activate_TX(uint8_t TX_ON);

//scanhead master Modbus functions
MB_Status_t MS_Transmit(uint8_t *data, uint16_t len);
MB_Status_t MS_StartListening();
MB_Status_t MS_Activate_TX(uint8_t TX_ON);
MB_Status_t MS_Request_Recieved(void *ptr);

//resets the program sequence
void Program_Reset();

void System_Init();

void Enable_Actuators(uint8_t Enable);
#endif /* INC_HW_INTERFACE_H_ */
