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

#define BQ29330_I2C_ADDR 0x10			// Dirección I2C del BQ29330
#define MAX_CELLS 5  					// Cantidad total de celdas
#define INA219_ADDRESS         (0x40 << 1)  // Dirección 7-bit del INA219
#define INA219_REG_BUS_VOLTAGE 0x02
#define INA219_REG_SHUNT_VOLTAGE 0x01
#define INA219_REG_CURRENT     0x04
#define INA219_REG_POWER       0x03

// Enumeración de registros con nombres representativos

typedef enum {
    BQ29330_STATUS_REG       = 0x00,
    BQ29330_CONTROL_REG      = 0x01,
    BQ29330_PROTECT1_REG     = 0x02,
    BQ29330_PROTECT2_REG     = 0x03,
    BQ29330_OVERCURRENT_REG  = 0x04,
    BQ29330_CELLBAL_REG      = 0x05,
    BQ29330_ADCONFIG_REG     = 0x06,
    BQ29330_ADCIN1_REG       = 0x07,
    BQ29330_ADCIN2_REG       = 0x08,
    BQ29330_CELL1_REG        = 0x09,
    BQ29330_CELL2_REG        = 0x0A,
    BQ29330_CELL3_REG        = 0x0B,
    BQ29330_CELL4_REG        = 0x0C
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
float INA219_ReadBusVoltage(void);
float INA219_ReadShuntVoltage(void);
float INA219_ReadCurrent(void);
float INA219_ReadPower(void);





#ifdef __cplusplus
}
#endif

#endif /* INC_BMS_H_ */
