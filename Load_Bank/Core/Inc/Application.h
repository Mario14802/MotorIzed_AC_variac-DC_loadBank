/*
 * Application.h
 *
 *  Created on: May 14, 2025
 *      Author: Ramy Sorial
 */

#ifndef INC_APPLICATION_H_
#define INC_APPLICATION_H_

#include "HW_Interface.h"
#include "stm32f1xx_hal.h"
#include "NVM_Variables.h"
#include "TypesAndRegMap.h"
#include "../../Drivers/Modbus_RTU_C_Lib/Modbus_Slave.h"
#include "../../Drivers/Modbus_RTU_C_Lib/Modbus_Master.h"
#include "../../Drivers/Modbus_RTU_C_Lib/Modbus_MISC.h"
#include "../../Drivers/PI/PI.h"
#include <math.h> //for fabs
/*
 * Variac Control IO ports
 */
#define Variac_Min_Switch HAL_GPIO_ReadPin(IN_0_GPIO_Port, IN_0_Pin)?0:1
#define Variac_Max_Switch HAL_GPIO_ReadPin(IN_1_GPIO_Port, IN_1_Pin)?0:1
#define Variac_CW(Enable) HAL_GPIO_WritePin(RELAY_0_GPIO_Port, RELAY_0_Pin, Enable)
#define Variac_CCW(Enable) HAL_GPIO_WritePin(RELAY_1_GPIO_Port, RELAY_1_Pin, Enable)
//#define Variac_CW(Enable) HAL_GPIO_WritePin(M_Variac_R_Port, M_Variac_R_Pin, Enable)
//#define Variac_CCW(Enable) HAL_GPIO_WritePin(M_Variac_L_Port, M_Variac_L_Pin, Enable)

#define Current_AC_S(Enable) HAL_GPIO_WritePin(RELAY_7_GPIO_Port, RELAY_7_Pin, Enable)
#define VOLTAGE_AC_S(Enable) HAL_GPIO_WritePin(RELAY_6_GPIO_Port, RELAY_6_Pin, Enable)




#define Variac_Test(GPIO_PORT,PIN,Enable) HAL_GPIO_WritePin(GPIO_PORT, PIN, Enable)
#define Variac_Switch_Read_Test(GPIO_PORT, PIN)  HAL_GPIO_ReadPin(GPIO_PORT, PIN)

typedef enum {
    AC_STATE_CALC = 0,
    AC_STATE_ON,
    AC_STATE_OFF
} AC_State_t;

#define Threshold 5  //to exhange between the relays of the current and voltage


#define ENABLE 1
#define DISABLE 0

extern PI_Handle_t PI;

#define kp 		1.0f
#define maxout  70.0f

/*
 * CT control Relay
 */
#define CT_10A_Enable(Enable) 	HAL_GPIO_WritePin(RELAY_2_GPIO_Port, RELAY_2_Pin, Enable)

void Application_Init();
void Application_Run();
#endif /* INC_APPLICATION_H_ */
