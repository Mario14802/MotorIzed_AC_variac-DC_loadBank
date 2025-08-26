#include "../ADS11xx/ADS11xx.h"

#define ADS11xx_Conv_Register 0
#define ADS11xx_Conf_Register 1
#define ADS11xx_Comp_L_Register 2
#define ADS11xx_Comp_H_Register 3

uint8_t ADS11xx_Init(ADS11xx_t *ads)
{
	return ADS11xx_WriteConfig(ads) == 1 ? ADS11xx_OK : ADS11xx_NOK;
}

uint8_t ADS11xx_WriteConfig(ADS11xx_t *ads)
{
	ads->ConfigReg = (ads->OS << 15) | (ads->MuxConfig << 12) | (ads->FSR << 9) | (ads->Single_Shot << 8) | (ads->SamplingRate << 5) | (ads->Comp_Mode << 4) | (ads->Comp_Pol << 3) | (ads->Comp_Lat << 2) | (ads->Comp_Que);
	ads->DataBuffer[0] = ADS11xx_Conf_Register;
	ads->DataBuffer[1] = (uint8_t) (ads->ConfigReg >> 8);
	ads->DataBuffer[2] = (uint8_t) (ads->ConfigReg);
	return ads->HW_Interface.I2C_Write(ads->Chip_Address, ads->DataBuffer, 3) == 1 ? ADS11xx_OK : ADS11xx_NOK;
}

uint8_t ADS11xx_Read(ADS11xx_t *ads, int16_t *value, uint8_t *Status)
{
	//perform a dummy write
	ads->DataBuffer[0] = ADS11xx_Conv_Register;
	uint8_t OK = ads->HW_Interface.I2C_Write(ads->Chip_Address, ads->DataBuffer, 1);

	//perform a read
	if (OK)
	{
		if (ads->HW_Interface.I2C_Read(ads->Chip_Address, ads->DataBuffer, 3) == 1)
		{
			*value = (ads->DataBuffer[0] << 8) | ads->DataBuffer[1];
			*Status = ads->DataBuffer[2];
			return ADS11xx_OK;
		}
		else return ADS11xx_NOK;
	}
	else
	{
		return ADS11xx_NOK;
	}
}

uint8_t ADS11xx_Conv_Cplt_SW(ADS11xx_t *ads, uint8_t *convStat)
{
	ads->DataBuffer[0] = ADS11xx_Conf_Register; //load the reg address
	uint8_t OK = ads->HW_Interface.I2C_Write(ads->Chip_Address, ads->DataBuffer, 1);

	OK = ads->HW_Interface.I2C_Read(ads->Chip_Address, ads->DataBuffer, 1);
	*convStat = (ads->DataBuffer[0] & 0x80) ? 1 : 0;
	return OK == 1 ? ADS11xx_OK : ADS11xx_NOK;
}
