/*
 * TypesAndRegMap.h
 *
 *  Created on: Jul 23, 2024
 *      Author: Ramy Sorial
 */

#ifndef INC_TYPESANDREGMAP_H_
#define INC_TYPESANDREGMAP_H_

#include "stdint.h"
#include "NVM_Variables.h"

/**
 *  Holding Registers Map
 */
#define hregs_reg_VarNumber   28                   // Protocol variable start index
#define HREGS_SIZE            (63 - hregs_reg_VarNumber)

/**
 *  Machine parameters mapped into Holding Registers
 */
typedef struct {
    /*** AC_Variac Setpoints ***/
    uint16_t AC_Current_SP;       // 0: AC current setpoint
    uint16_t AC_Voltage_SP;       // 1: AC voltage setpoint

    /*** Load Bank: Write Holding_Reg Data ***/
    uint16_t Slave_address;       // 2: Slave address
    uint16_t Operating_mode;      // 3: Measurement mode
    uint16_t Test_Enable_Disable; // 4: Measurement on/off
    uint16_t CV_set_value;        // 5: Voltage limit
    uint16_t CI_set_value;        // 6: Current limit
    uint16_t CP_set_value;        // 7: Constant power limit
    uint16_t CR_set_value;        // 8: Constant resistance limit
    uint16_t Options;             // 9: Operating options
    uint16_t Baud;                // 10: Communication baud rate
    uint16_t Corr_MV;             // 11: Voltage correction factor
    uint16_t Corr_TV;             // 12: Test voltage correction
    uint16_t Corr_TF;             // 13: Test current correction
    uint16_t Corr_LF;             // 14: Load current correction

    /*** Load Bank: Read Holding_Reg Data ***/
    uint16_t Read_Slave_address;  // 15: Slave address (readback)
    uint16_t Read_Operating_mode; // 16: Measurement mode (readback)
    uint16_t Read_Test_Enable_Disable; // 17: Measurement on/off (readback)
    uint16_t Read_CV_set_value;   // 18: Voltage limit (readback)
    uint16_t Read_CI_set_value;   // 19: Current limit (readback)
    uint16_t Read_CP_set_value;   // 20: Constant power limit (readback)
    uint16_t Read_CR_set_value;   // 21: Constant resistance limit (readback)
    uint16_t Read_Options;        // 22: Operating options (readback)
    uint16_t Read_Baud;           // 23: Baud rate (readback)
    uint16_t Read_Corr_MV;        // 24: Voltage correction (readback)
    uint16_t Read_Corr_TV;        // 25: Test voltage correction (readback)
    uint16_t Read_Corr_TF;        // 26: Test current correction (readback)
    uint16_t Read_Corr_LF;        // 27: Load current correction (readback)

    uint16_t Free[HREGS_SIZE];    // Unused registers
    uint16_t Checksum;            // Final checksum
} MachineParams;

/**
 * the machine parameters and states are mapped directly into the
 * holding register map accessible through the Modbus
 * double-word variables should always start with an even address number
 */
typedef struct
{
    /*************** Machine Params - non volatile ********************/
    /// occupying a total of 64-words (128 bytes)
    MachineParams Params;

    ///RW states starting from address 64
    uint16_t Temp; //64
} HoldingRegMap;




/**
 *  Input Registers Map
 */
#define inputreg_VarNumber 22
#define IREGS_SIZE         (63 - inputreg_VarNumber)

typedef struct {
    /*** Multimeter Data ***/
    uint16_t AC_Voltage_H;          // 0: AC voltage higher than 5V
    uint16_t AC_Current_H;          // 1: Measured AC current >5
    uint16_t AC_Power;            // 2: Measured AC power
    uint16_t AC_Freq;             // 3: Measured AC frequency

    /*** Load Bank Readback ***/
    uint16_t Slave_address;       // 4: Slave address
    uint16_t Operating_mode;      // 5: Measurement mode
    uint16_t Test_Enable_Disable; // 6: Measurement on/off
    uint16_t Temp_Q1;             // 7: Temperature sensor Q1
    uint16_t Temp_Q2;             // 8: Temperature sensor Q2
    uint16_t In_Volt;             // 9: Input voltage
    uint16_t DC_Voltage;          // 10: DC output voltage
    uint16_t DC_Current;          // 11: DC output current
    uint16_t DC_Power;            // 12: DC output power
    uint16_t Q_Block;             // 13: Block charge value
    uint16_t Q_Cap;               // 14: Capacity
    uint16_t Q_Wh;                // 15: Watt-hour value
    uint16_t OK_Time;             // 16: Operation OK duration
 //   uint16_t AC_Voltage;        // 17: AC voltage

    int16_t AC_V_BELOW_5V; 		  //17 :AC voltage below 5 volts
    uint16_t AC_Input_Voltage;	  //18 : Input AC voltage
    uint16_t AC_Current_L;  	  //19 : Measured AC current <

    uint16_t AC_Voltage; //20
    uint16_t AC_Current ;//21

    uint16_t Extras[IREGS_SIZE];  // Unused registers
} InputRegMap;

/*** Bit Maps ***/
enum InputBitMap {
    Multimeter_COMM_OK = 0,
    LOAD_BANK_COMM_OK   = 0
};

enum CoilBitMap {
    Enable_System,
    Disable_System
};

/*** External References ***/
extern HoldingRegMap *Hregs;
extern InputRegMap   *Iregs;

/*** Bit Manipulation Macros ***/
#define SetRegBit(Reg, bit)      ((Reg) |= (bit))
#define ResetRegBit(Reg, bit)    ((Reg) &= ~(bit))
#define GetRegBit(Reg, bit)      (((Reg) & (bit)) ? 1 : 0)

#define _MB_GetCoil(coil)         MB_Parse_Bit(MB.CoilBits, coil)
#define _MB_SetCoil(coil, state)  MB_Encode_Bit(MB.CoilBits, coil, state)
#define _MB_SetInput(input, state) MB_Encode_Bit(MB.InputBits, input, state)
#define _MB_GetInput(input)       MB_Parse_Bit(MB.InputBits, input)

#define _PC_GetCoil(coil)         MB_Parse_Bit(MB.CoilBits, coil)
#define _PC_SetCoil(coil, state)  MB_Encode_Bit(MB.CoilBits, coil, state)
#define _PC_SetInput(input, state) MB_Encode_Bit(MB.InputBits, input, state)

#define HoldingRegsSize  (sizeof(HoldingRegMap) / 2)
#define InputRegsSize  (sizeof(InputRegMap) / 2)

#endif /* INC_TYPESANDREGMAP_H_ */
