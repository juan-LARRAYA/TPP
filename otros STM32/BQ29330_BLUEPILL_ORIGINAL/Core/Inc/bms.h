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


#define INA219_REG_BUS_VOLTAGE 0x02
#define INA219_REG_SHUNT_VOLTAGE 0x01
#define INA219_REG_CURRENT     0x04
#define INA219_REG_POWER       0x03

// Enumeración de registros con nombres representativos

typedef enum {
    BQ29330_STATUS      		= 0x00,
    BQ29330_OUTPUT_CONTROL     	= 0x01,
    BQ29330_STATE_CONTROL    	= 0x02,
    BQ29330_FUNCTION_CONTROL    = 0x03,
    BQ29330_CELL  				= 0x04,
    BQ29330_OLV      			= 0x05,
    BQ29330_OLD     			= 0x06,
	BQ29330_SCC     			= 0x07,
    BQ29330_SCD     			= 0x08
} BQ29330_Registers;

// Estructura para almacenar datos del BQ29330
typedef struct {
    I2C_HandleTypeDef *hi2c;  // Interfaz I2C utilizada

    // Voltajes de celdas
    uint16_t cell_voltages[5];   // Almacena voltajes de hasta 5 celdas

    // Voltajes generales
    uint16_t stack_voltage;      // Voltaje total del pack

    // Temperatura
    int16_t internal_temperature; // Temperatura interna del BQ29330
    uint16_t ts_measurement;      // Medición del sensor térmico externo

    // Corriente
    int32_t raw_current;         // Medición de corriente cruda en 32 bits
    int16_t current;             // Medición de corriente CC2 en 16 bits

    // Estados y protecciones
    uint8_t battery_status;      // Estado general de la batería
    uint8_t alarm_status;        // Estado de alarmas activas
    uint8_t alarm_raw_status;    // Estado crudo de alarmas
    uint8_t enabled_protections; // Protecciones activas

    // Configuración de protección por voltaje
    uint16_t cell_undervoltage_threshold;
    uint16_t cell_overvoltage_threshold;

    // Protección por sobrecorriente
    uint8_t overcurrent_charge_threshold;
    uint8_t overcurrent_discharge_1;
    uint8_t overcurrent_discharge_2;
    uint8_t short_circuit_discharge;

    // Protección por temperatura
    uint8_t overtemp_charge_threshold;
    uint8_t undertemp_charge_threshold;
    uint8_t overtemp_discharge_threshold;
    uint8_t undertemp_discharge_threshold;
    uint8_t internal_overtemp_threshold;

    //registros
    uint8_t fet_control;


} BQ29330_Device;



// Funciones para configurar y leer datos del BQ29330

void sendBMSDataI2C(BQ29330_Device *bms);
uint16_t INA219_ReadBusVoltage(void);
uint16_t INA219_ReadShuntVoltage(void);
uint16_t INA219_ReadCurrent(void);
uint16_t INA219_ReadPower(void);
HAL_StatusTypeDef BQ29330_ReadRegister(BQ29330_Registers reg, uint8_t *rxData);
HAL_StatusTypeDef BQ29330_WriteRegister(BQ29330_Registers reg, uint8_t data);
HAL_StatusTypeDef BQ29330_ReadFunctionControl(BQ29330_Registers reg, uint8_t *valor_leido);




#ifdef __cplusplus
}
#endif

#endif /* INC_BMS_H_ */
