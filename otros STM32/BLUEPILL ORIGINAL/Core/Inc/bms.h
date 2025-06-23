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


    // Estados y protecciones
    uint8_t BQ29330_status;           // Estado de fallas
    uint8_t BQ29330_output_countrol;  // Control de salidas
    uint8_t BQ29330_state_countrol;   // Control de modos
    uint8_t BQ29330_function_control;   // Control de modos
    uint8_t BQ29330_cell;             // Celda o modo seleccionado
    uint8_t BQ29330_OLV;              // Umbral de sobrecorriente
    uint8_t BQ29330_OLD;              // Retardo sobrecorriente
    uint8_t BQ29330_SCC;              // Corte por cortocircuito en carga
    uint8_t BQ29330_SCD;              // Corte por cortocircuito en descarga

    // Corrientes de protección
    uint8_t CC_charge;                // Umbral de carga
    uint8_t CC_discharge;             // Umbral de descarga
    uint8_t overload_voltage;         // Sobrevoltaje


} BQ29330_Device;



// Funciones para configurar y leer datos del BQ29330

void sendBMSDataI2C(BQ29330_Device *bms);

HAL_StatusTypeDef BQ29330_ReadRegister(BQ29330_Registers reg, uint8_t *rxData);
HAL_StatusTypeDef BQ29330_WriteRegister(BQ29330_Registers reg, uint8_t data);
HAL_StatusTypeDef BQ29330_ReadFunctionControl(BQ29330_Registers reg, uint8_t *valor_leido);
void BQ29330_config();




#ifdef __cplusplus
}
#endif

#endif /* INC_BMS_H_ */
