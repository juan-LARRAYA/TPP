/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DELAY 1000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t convCompleted=0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	convCompleted=1;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_USB_DEVICE_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  //start PWMs and set the duty to 50%
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, htim1.Init.Period/2);


  //ADC Variables
   uint16_t rawValues[2];
   HAL_ADC_Start_DMA(&hadc1,(uint32_t *) rawValues, 3);

   //BMS
   BQ29330_WriteRegister(BQ29330_OUTPUT_CONTROL,0x06);
   BQ29330_Device bq = { .hi2c = &hi2c1 };

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	//read all adcs
	while(!convCompleted);
	//factor de multiplicacion de la corriente en entradas mppt (50 x 33mohm)^-1
	//las tensiones se multiplican x2
	uint16_t cell_mas = rawValues[0] * CONVERSION_FACTOR;
	uint16_t cell_menos = rawValues[1] * CONVERSION_FACTOR;
	uint16_t difference = BMSVoltageCorrection(cell_mas - cell_menos); //correcion tener la tension de la celda

	// Imprimo cosas
	char buffer[STR_LEN];
	snprintf(buffer, STR_LEN, "\n \n I LIKE THE WAY YOU WORKING \n");
	HAL_I2C_Master_Transmit(&hi2c1, ARDUINO_I2C_ADDRESS << 1, (uint8_t *) buffer, strlen(buffer), HAL_MAX_DELAY);
/*
    sprintf(buffer,"Corriente: %u \n", current);
	HAL_I2C_Master_Transmit(&hi2c1, ARDUINO_I2C_ADDRESS << 1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
*/
	//FORMA NUEVA DE IMPRIMIR LLAMANDO A UNA FUNCION SI NO ANDA USAR LA FORMA VIEJA
	sendUsartMsg("Tension cell +: ", rawValues[0]);
	sendUsartMsg("Tension cell -: ", rawValues[1]);
	sendUsartMsg("Tension celda: ", difference);



	//BMS 29330

    //escribo
    BQ29330_WriteRegister(BQ29330_STATE_CONTROL, 0x0C);   // WDDIS = 1, all else = 0

    BQ29330_WriteRegister(BQ29330_FUNCTION_CONTROL, 0x01); // VMEN||BAT = 0
    BQ29330_WriteRegister(BQ29330_CELL, n); 			   //
    BQ29330_WriteRegister(BQ29330_OLV, 0x00);              // 50 mV 0x00
    BQ29330_WriteRegister(BQ29330_OLD, 0x0F);              // 31 ms
    BQ29330_WriteRegister(BQ29330_SCC, 0x00);              // 475 mV y 915 μs
    BQ29330_WriteRegister(BQ29330_SCD, 0x00);              // idem for discharge
    n++;
    n = n%2;


    //leo
	HAL_StatusTypeDef statusI2c = BQ29330_ReadFunctionControl(BQ29330_STATUS, &bq.BQ29330_status);

	//LEO el registro que escribi
	statusI2c =  BQ29330_ReadFunctionControl(BQ29330_OUTPUT_CONTROL, &bq.BQ29330_output_countrol);
	statusI2c =  BQ29330_ReadFunctionControl(BQ29330_STATE_CONTROL, &bq.BQ29330_state_countrol);
	statusI2c =  BQ29330_ReadFunctionControl(BQ29330_FUNCTION_CONTROL, &bq.BQ29330_function_control);
	statusI2c =  BQ29330_ReadFunctionControl(BQ29330_CELL, &bq.BQ29330_cell);
	statusI2c =  BQ29330_ReadFunctionControl(BQ29330_OLV, &bq.BQ29330_OLV);
	statusI2c =  BQ29330_ReadFunctionControl(BQ29330_OLD, &bq.BQ29330_OLD);
	statusI2c =  BQ29330_ReadFunctionControl(BQ29330_SCC, &bq.BQ29330_SCC);
	statusI2c =  BQ29330_ReadFunctionControl(BQ29330_SCD, &bq.BQ29330_SCD);

	sendUsartMsg("statusI2c : ", statusI2c);
	sendUsartMsg("celda medida : ", n + 1);

	/*
	sendUsartMsg("Estatus del bms : ", bq.BQ29330_status);
	sendUsartMsg("OUTPUT_CONTROL : ", bq.BQ29330_output_countrol);


	//sendUsartMsg("STATE_CONTROL : ", bq.BQ29330_state_countrol);

	sendUsartMsg("FUNCTION_CONTROL : ", bq.BQ29330_function_control);
	sendUsartMsg("CELL : ", bq.BQ29330_cell);
	sendUsartMsg("OLV (Overload voltage threshold): ", bq.BQ29330_OLV);
	sendUsartMsg("OLD (Overload delay time): ", bq.BQ29330_OLD);
	sendUsartMsg("SCC (Short circuit in charge): ", bq.BQ29330_SCC);
	sendUsartMsg("SCD (Short circuit in discharge): ", bq.BQ29330_SCD);
	 */

	//si hay una fall reiniciar el bq
	if(bq.BQ29330_status & (1U << 4) || (bq.BQ29330_status && bq.BQ29330_status != 16)){
	    BQ29330_WriteRegister(BQ29330_OUTPUT_CONTROL,0x07);
	    BQ29330_WriteRegister(BQ29330_OUTPUT_CONTROL,0x06);
		statusI2c =  BQ29330_ReadFunctionControl(BQ29330_STATUS, &bq.BQ29330_status);
	    sendUsartMsg("Estatus del bms despues del latch: ", bq.BQ29330_status);
    }

	sendUsartMsg(" ", 9999); //renglon en blanco



/*

	uint16_t shuntVoltage = INA219_ReadShuntVoltage();
	uint16_t busVoltage = INA219_ReadBusVoltage();
	uint16_t current_mA = INA219_ReadCurrent();
	uint16_t power_mW = INA219_ReadPower();
	uint16_t loadVoltage = busVoltage + (shuntVoltage / 1000.0f);
	sendI2CMsg("Bus Voltage: ", busVoltage);
	sendI2CMsg("Shunt Voltage: ", shuntVoltage);
	sendI2CMsg("Load Voltage: ", loadVoltage);
	sendI2CMsg("Current: ", current_mA);
	sendI2CMsg("Power: ", power_mW);

*/




	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	HAL_Delay(DELAY);

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV4;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
