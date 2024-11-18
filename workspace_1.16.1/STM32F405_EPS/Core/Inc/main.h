/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define CurrXMPPT_Pin GPIO_PIN_0
#define CurrXMPPT_GPIO_Port GPIOC
#define VoltXMPPT_Pin GPIO_PIN_1
#define VoltXMPPT_GPIO_Port GPIOC
#define CurrYMPPT_Pin GPIO_PIN_2
#define CurrYMPPT_GPIO_Port GPIOC
#define VoltYMPPT_Pin GPIO_PIN_3
#define VoltYMPPT_GPIO_Port GPIOC
#define MPPTYControl_Pin GPIO_PIN_0
#define MPPTYControl_GPIO_Port GPIOA
#define CurrZMPPT_Pin GPIO_PIN_1
#define CurrZMPPT_GPIO_Port GPIOA
#define VoltZMPPT_Pin GPIO_PIN_2
#define VoltZMPPT_GPIO_Port GPIOA
#define MPPTZControl_Pin GPIO_PIN_3
#define MPPTZControl_GPIO_Port GPIOA
#define Curr5VBis_Pin GPIO_PIN_4
#define Curr5VBis_GPIO_Port GPIOA
#define Volt5VBis_Pin GPIO_PIN_5
#define Volt5VBis_GPIO_Port GPIOA
#define Volt3_3V_Pin GPIO_PIN_6
#define Volt3_3V_GPIO_Port GPIOA
#define Curr3_3V_Pin GPIO_PIN_7
#define Curr3_3V_GPIO_Port GPIOA
#define Volt5V_Pin GPIO_PIN_4
#define Volt5V_GPIO_Port GPIOC
#define Curr5V_Pin GPIO_PIN_5
#define Curr5V_GPIO_Port GPIOC
#define Volt3_3VBis_Pin GPIO_PIN_0
#define Volt3_3VBis_GPIO_Port GPIOB
#define Curr3_3Vbis_Pin GPIO_PIN_1
#define Curr3_3Vbis_GPIO_Port GPIOB
#define Enable5V_Pin GPIO_PIN_2
#define Enable5V_GPIO_Port GPIOB
#define Enable3_3VBis_Pin GPIO_PIN_10
#define Enable3_3VBis_GPIO_Port GPIOB
#define Enable3_3V_Pin GPIO_PIN_11
#define Enable3_3V_GPIO_Port GPIOB
#define Enable5VBis_Pin GPIO_PIN_12
#define Enable5VBis_GPIO_Port GPIOB
#define EnableVbat_Pin GPIO_PIN_14
#define EnableVbat_GPIO_Port GPIOB
#define SDA_Sat_Pin GPIO_PIN_9
#define SDA_Sat_GPIO_Port GPIOC
#define SCL_Sat_Pin GPIO_PIN_8
#define SCL_Sat_GPIO_Port GPIOA
#define USB_D__Pin GPIO_PIN_11
#define USB_D__GPIO_Port GPIOA
#define USB_D_A12_Pin GPIO_PIN_12
#define USB_D_A12_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define TX_Pin GPIO_PIN_10
#define TX_GPIO_Port GPIOC
#define RX_Pin GPIO_PIN_11
#define RX_GPIO_Port GPIOC
#define SCL_BMS_Pin GPIO_PIN_6
#define SCL_BMS_GPIO_Port GPIOB
#define SDA_BMS_Pin GPIO_PIN_7
#define SDA_BMS_GPIO_Port GPIOB
#define BMS_Alert_Pin GPIO_PIN_8
#define BMS_Alert_GPIO_Port GPIOB
#define MPPTXControl_Pin GPIO_PIN_9
#define MPPTXControl_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
