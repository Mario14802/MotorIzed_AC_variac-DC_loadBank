#ifndef _ADS11xx_H
#define _ADS11xx_H

#include <stdint.h>

/// @enum
/// @brief
/// defines the various slave address configs
/// for various chip suffixes (ADS1100) and for
/// various AD0 pin connection (ADS1115 like devices)
typedef enum
{
	ADS11xx_AD0_GND = 0b01001000, ///< ADS11xx_AD0_GND
	ADS11xx_AD1_VDD = 0b01001001, ///< ADS11xx_AD1_VDD
	ADS11xx_AD2_SDA = 0b01001010, ///< ADS11xx_AD2_SDA
	ADS11xx_AD3_SCL = 0b01001001, ///< ADS11xx_AD3_SCL
} ADS11xx_SLA;

/// @enum
/// @brief
/// Defines the various MUS configs
/// In+ _ In- Connection
/// Applicable only for multi-channel devices
typedef enum
{
	ADS11xx_MUX_AIN0_AIN1 = 0b000,
	ADS11xx_MUX_AIN0_AIN3 = 0b001,
	ADS11xx_MUX_AIN1_AIN3 = 0b010,
	ADS11xx_MUX_AIN2_AIN3 = 0b011,
	ADS11xx_MUX_AIN0_GND = 0b100,
	ADS11xx_MUX_AIN1_GND = 0b101,
	ADS11xx_MUX_AIN2_GND = 0b110,
	ADS11xx_MUX_AIN3_GND = 0b111,
} ADS11xx_MUX_Config;

#define ADS11xx_OK  1
#define ADS11xx_NOK 0

typedef enum
{
	ADS11xx_8SPS,
	ADS11xx_16SPS,
	ADS11xx_32SPS,
	ADS11xx_64SPS,
	ADS11xx_128SPS,
	ADS11xx_250SPS,
	ADS11xx_475SPS,
	ADS11xx_860SPS,
} ADS11xx_Rate_t;

typedef enum
{
	ADS11xx_FSR_6_14,
	ADS11xx_FSR_4_096,
	ADS11xx_FSR_2_048,
	ADS11xx_FSR_1_024,
	ADS11xx_FSR_0_512,
	ADS11xx_FSR_0_256,
} ADS11xx_PGA;

typedef struct ADS11xx_t
{
	/// @brief defines the chip suffix and hence the appropriate slave address
	ADS11xx_SLA Chip_Address;

	ADS11xx_PGA FSR;
	ADS11xx_MUX_Config MuxConfig;
	ADS11xx_Rate_t SamplingRate;

	/// @brief set to 1 for single shot conversion mode
	uint8_t Single_Shot :1;

	/// @brief set to 1 for Window mode, 0 for Traditional mode
	uint8_t Comp_Mode :1;

	/// @brief sets the comparator polarity (1 for active high)
	uint8_t Comp_Pol :1;

	/// @brief set to 1 for latching mode
	uint8_t Comp_Lat :1;

	/// @brief set to to trigger a conversion
	uint8_t OS:1;

	/// @brief sets the queue mode:
	/// 00 : Assert after 1 conversion
	/// 01 : Assert after 2 conversions
	/// 10 : Assert after 4 conversions
	///	11 : Comparator Disabled
	uint8_t Comp_Que :2;

	/// @brief Comparator low threshold register
	uint16_t Comp_L_Thresh;
	/// @brief Comparator high threshold register
	uint16_t Comp_H_Thresh;

	struct
	{
		/// @fn uint8_t (*)(uint8_t, uint16_t, uint8_t*, uint16_t)
		/// @brief
		/// Writes bytes
		/// @param SLA 7-bit slave address
		/// @param Data ptr to the data buffer to be written
		/// @param len length of the data to be written (excl the address byte)
		/// @return
		uint8_t (*I2C_Write)(uint8_t SLA, uint8_t *Data, uint8_t len);

		/// @fn uint8_t (*)(uint8_t, uint16_t, uint8_t*, uint16_t)
		/// @brief
		/// Reads bytes
		/// @param SLA 7-bit slave address
		/// @param Data ptr to the data buffer to be read into
		/// @param len length of the data to be read from the salve device
		/// @return
		uint8_t (*I2C_Read)(uint8_t SLA, uint8_t *Data, uint8_t len);

	} HW_Interface;

	/// @brief Intermediate buffer used for communication
	uint8_t DataBuffer[5];

	/// @brief Intermediate variable to hold the config data
	uint16_t ConfigReg;
} ADS11xx_t;

#ifdef __cplusplus
extern "C"
{
#endif

/// @brief Initializes and configures the chip
/// @param ads ptr to the chip handle
/// @param PGA_Setting setting for the programmable gain amplifier
/// @param Sampling_Rate settings for the sampling rate
/// @return ADS11xx_OK if OK
uint8_t ADS11xx_Init(ADS11xx_t *ads);

/// @brief
/// writes the chip configuration to the IC
/// @param ads ptr to the chip handle
/// @return ADS11xx_OK if OK
uint8_t ADS11xx_WriteConfig(ADS11xx_t *ads);

/// @fn uint8_t ADS11xx_Conv_Cplt_SW(ADS11xx_t*, uint8_t*)
/// @brief
/// Checks if the ADC conversion is complete
/// @param ads ptr to the chip handle
/// @param convStat ptr to the variable holding the stat, 1 for complete
/// @return ADX11xx_OK if OK
uint8_t ADS11xx_Conv_Cplt_SW(ADS11xx_t *ads, uint8_t *convStat);

/// @brief Reads the analog result
/// @param ads Ptr to the chip handle
/// @param value ptr to where the value should be stored
/// @param Status ptr to where the status dta will be placed
/// @return 1 if successfully read the analog value
uint8_t ADS11xx_Read(ADS11xx_t *ads, int16_t *value, uint8_t *Status);

#ifdef __cplusplus
}
#endif

#endif
