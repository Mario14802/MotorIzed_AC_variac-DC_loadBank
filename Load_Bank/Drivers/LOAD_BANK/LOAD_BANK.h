/*
 * LOAD_BANK.h
 *
 *  Created on: Jul 2, 2025
 *      Author: MARIO
 */

#ifndef LOAD_BANK_LOAD_BANK_H_
#define LOAD_BANK_LOAD_BANK_H_

#include "../Modbus_RTU_C_Lib/Modbus_Master.h"




typedef enum {
    /* ———— Holding Registers (0x0000 … 0x0012) ———— */
	HREG_SLAVE_ADD 									 =0,	  //slave addresse
    HREG_MEASUREMENT_OPERATING_MODE,                   		  // Measurement Quantity mode
    HREG_MEASUREMENT_TEST_ENABLE,                        	   // Measurement quantity switch (40003)
    HREG_SET_VOLTAGE_VALUE,                                   // Voltage value                     (40004)
    HREG_SET_CURRENT,                              			  // Electricity flow value            (40005)
    HREG_SET_CONSTANT_POWER_VALUE,                                // Constant power value              (40006)
    HREG_SET_RESISTANCE_VALUE,                                   // Electricity value                 (40007)
    HREG_OPERATE_OPTIONS,                                     // Operate options                   (40008)
    HREG_PASS_NEWS_BAUD_RATE,                                 // Pass news baud rate               (40009)
    HREG_VOLTAGE_VALUE_OFFSET,                   // Correction Measured voltage value (40010)
    HREG_CURRENT_VALUE_OFFSET,                       // Correction Test voltage value     (40011)
    HREG_POWER_VALUE_OFFSET,              // Correction test electricity flow  (40012)
    HREG_ENERGY_VALUE_OFFSET,          // Correction lose out electricity   (40013)

    /* ———— Input Registers (0x0000 … 0x0012) ———— */
	IREG_SLAVE_ADD 									 =0,	   //slave addresse
    IREG_MEASUREMENT_QUANTITY_MODE,  						   // Measurement Quantity mode
    IREG_MEASUREMENT_QUANTITY_SWITCH,                          // Measurement quantity switch       (30003)
    IREG_TEMPERATURE_SPEND_MEASUREMENT_QUANTITY,               // temperature Spend Measurement     (30004)
    IREG_TEMPERATURE_SPEND_MEASUREMENT_QUANTITY_2,             // temperature Spend Measurement     (30005)
    IREG_MEASUREMENT_QUANTITY_VOLTAGE,                         // Measurement quantity Voltage      (30006)
    IREG_QUIET_MEASURING_VOLTAGE,                              // quilt Measuring voltage           (30007)
    IREG_QUIET_MEASURING_ELECTRICITYFLOW,                      // quilt Measuring electricityflow   (30008)
    IREG_QUIET_MEASUREMENT_POWER,                              // quilt Measurement power           (30009)
    IREG_QUIET_MEASUREMENT_BLOCKVALUE,                         // quilt Measurement blockvalue      (30010)
    IREG_QUIET_MEASUREMENT_CAPACITY,                           // quilt Measurement capacity        (30011)
    IREG_QUIET_MEASUREMENT_WATTHOUR,                           // quilt Measurement watthour        (30012)
    IREG_TRANSPORT_OK_TIME                                     // transport OK time                 (30013)
} LOAD_BANK_CMD_t;


typedef enum REG_Type
{
	INPUT_REG,
	HOLDING_REG_Read,
	HOLDING_REG_Write

}REG_Type_t;




typedef struct LOAD_BANK_t
{
	uint8_t Comm_OK :1; //1 for COMM ok
	uint8_t User_CMD_Cplt :1;
	uint8_t User_CMD_Success :1;

	uint8_t SLA;



	uint8_t Data_Type;

	MB_Master_t *bus;


	int16_t Seq;
	LOAD_BANK_CMD_t Current_CMD;
	uint16_t *p_data;
	uint16_t Data;
	uint8_t NumOfWords;
} LOAD_BANK_t;


void LOAD_BANK_Init(LOAD_BANK_t *LB);

uint8_t LOAD_BANK_Read(LOAD_BANK_t *LB, LOAD_BANK_CMD_t Command, uint8_t NumOfWords, uint16_t *data,REG_Type_t Type);

uint8_t LOAD_BANK_Write(LOAD_BANK_t *LB, LOAD_BANK_CMD_t Command, uint16_t data);



void LOAD_BANK_Routine(LOAD_BANK_t *LB);


#endif /* LOAD_BANK_LOAD_BANK_H_ */
