/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define M_Variac_R_Port	GPIOA
#define M_Variac_L_Port	GPIOA
#define M_Variac_R_Pin	GPIO_PIN_0
#define M_Variac_L_Pin	GPIO_PIN_1

#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define IN_6_Pin GPIO_PIN_14
#define IN_6_GPIO_Port GPIOC
#define IN_7_Pin GPIO_PIN_15
#define IN_7_GPIO_Port GPIOC
#define ENC_1_Pin GPIO_PIN_0
#define ENC_1_GPIO_Port GPIOA
#define ENC_B_Pin GPIO_PIN_1
#define ENC_B_GPIO_Port GPIOA
#define M_variac_R_Pin GPIO_PIN_4
#define M_variac_R_GPIO_Port GPIOA
#define M_variac_L_Pin GPIO_PIN_5
#define M_variac_L_GPIO_Port GPIOA
#define IN_2_Pin GPIO_PIN_6
#define IN_2_GPIO_Port GPIOA
#define IN_3_Pin GPIO_PIN_7
#define IN_3_GPIO_Port GPIOA
#define RELAY_0_Pin GPIO_PIN_0
#define RELAY_0_GPIO_Port GPIOB
#define RELAY_1_Pin GPIO_PIN_1
#define RELAY_1_GPIO_Port GPIOB
#define BOOT_1_Pin GPIO_PIN_2
#define BOOT_1_GPIO_Port GPIOB
#define RELAY_2_Pin GPIO_PIN_12
#define RELAY_2_GPIO_Port GPIOB
#define RELAY_3_Pin GPIO_PIN_13
#define RELAY_3_GPIO_Port GPIOB
#define RELAY_4_Pin GPIO_PIN_14
#define RELAY_4_GPIO_Port GPIOB
#define RELAY_5_Pin GPIO_PIN_15
#define RELAY_5_GPIO_Port GPIOB
#define RELAY_6_Pin GPIO_PIN_8
#define RELAY_6_GPIO_Port GPIOA
#define IN_4_Pin GPIO_PIN_11
#define IN_4_GPIO_Port GPIOA
#define IN_5_Pin GPIO_PIN_12
#define IN_5_GPIO_Port GPIOA
#define ADS_RDY_Pin GPIO_PIN_15
#define ADS_RDY_GPIO_Port GPIOA
#define RS_R_EN_Pin GPIO_PIN_3
#define RS_R_EN_GPIO_Port GPIOB
#define RS_W_EN_Pin GPIO_PIN_4
#define RS_W_EN_GPIO_Port GPIOB
#define RELAY_7_Pin GPIO_PIN_5
#define RELAY_7_GPIO_Port GPIOB
#define PWM_2_Pin GPIO_PIN_8
#define PWM_2_GPIO_Port GPIOB
#define PWM_1_Pin GPIO_PIN_9
#define PWM_1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
