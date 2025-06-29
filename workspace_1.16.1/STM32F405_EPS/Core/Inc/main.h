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
#include <string.h>
#include <stdio.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define ARDUINO_I2C_ADDRESS 0x08  		// Dirección I2C del Arduino
#define STR_LEN 200


/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MPPTYControl_Pin GPIO_PIN_0
#define MPPTYControl_GPIO_Port GPIOA
#define MPPTZControl_Pin GPIO_PIN_3
#define MPPTZControl_GPIO_Port GPIOA
#define Enable5V_Pin GPIO_PIN_2
#define Enable5V_GPIO_Port GPIOB
#define Enable3_3VBis_Pin GPIO_PIN_10
#define Enable3_3VBis_GPIO_Port GPIOB
#define Enable3_3V_Pin GPIO_PIN_11
#define Enable3_3V_GPIO_Port GPIOB
#define Enable5VBis_Pin GPIO_PIN_12
#define Enable5VBis_GPIO_Port GPIOB
#define EnableCalefactor_Pin GPIO_PIN_14
#define EnableCalefactor_GPIO_Port GPIOB
#define SDA_I2C_COM_Pin GPIO_PIN_9
#define SDA_I2C_COM_GPIO_Port GPIOC
#define SCL_I2C_COM_Pin GPIO_PIN_8
#define SCL_I2C_COM_GPIO_Port GPIOA
#define EnableBatOut_Pin GPIO_PIN_9
#define EnableBatOut_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define TX_Pin GPIO_PIN_10
#define TX_GPIO_Port GPIOC
#define RX_Pin GPIO_PIN_11
#define RX_GPIO_Port GPIOC
#define SCL_I2C_BMS_Pin GPIO_PIN_6
#define SCL_I2C_BMS_GPIO_Port GPIOB
#define SDA_I2C_BMS_Pin GPIO_PIN_7
#define SDA_I2C_BMS_GPIO_Port GPIOB
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
