/*
 * bms.h
 *
 *  Created on: Mar 2, 2025
 *      Author: apple
 */

#ifndef INC_BMS_H_
#define INC_BMS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "i2c.h"
#include "main.h"
#include <stdint.h>

#define BQ76905_I2C_ADDR 0x10	// Dirección I2C del BQ76905
#define FET_CONTROL_CMD 0x29	// Definición del comando de FET Control

// Estructura para almacenar datos de la batería
typedef struct {
    uint16_t cell1_voltage;
    uint16_t cell2_voltage;
    uint8_t alert_status_A;
    uint8_t fault_status_A;
    uint8_t alert_status_B;
    uint8_t fault_status_B;
    uint16_t battery_status;
} BatteryData;

// Declaraciones de funciones
HAL_StatusTypeDef BQ76905_WriteRegister(uint8_t reg, uint8_t* data, uint16_t len);
HAL_StatusTypeDef BQ76905_ReadRegister(uint8_t reg, uint8_t* data, uint16_t len);
HAL_StatusTypeDef BQ76905_ReadRegister_test(uint8_t regAddr, uint8_t *data, uint16_t size);
void ReadCellVoltage(I2C_HandleTypeDef *hi2c, uint8_t cell);
void enable_Discharge_FET(void);
void disable_Discharge_FET(void);






#ifdef __cplusplus
}
#endif

#endif /* INC_BMS_H_ */
