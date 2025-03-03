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


// Enumeración de registros con nombres representativos
typedef enum {
    BASE_SUBCOMMAND_ADDRESS  = 0x3E,   // Dirección base de subcomandos

    // Configuración y estado
    CONFIG_UPDATE            = 0x0090, // Entrar en modo configuración
    CONFIG_EXIT              = 0x0092, // Salir de modo configuración
    VCELL_MODE               = 0x901B, // Configurar cantidad de celdas en el sistema
    ENABLED_PROT_A           = 0x9024, // Protecciones habilitadas (Overcurrent, Short Circuit, etc.)
    CELL_UV_THRESHOLD        = 0x902E, // Umbral de bajo voltaje de celda
    BATTERY_STATUS           = 0x12,   // Estado de la batería
    ALARM_ENABLE             = 0x66,   // Habilitar alarmas
    FET_CONTROL              = 0x68,   // Control de FETs

    // Voltajes de celdas (mV)
    CELL1_VOLTAGE            = 0x14,
    CELL2_VOLTAGE            = 0x16,
    CELL3_VOLTAGE            = 0x18,
    CELL4_VOLTAGE            = 0x1A,
    CELL5_VOLTAGE            = 0x1C,

    // Otros voltajes
    STACK_VOLTAGE            = 0x26,   // Voltaje de la pila completa (pack de baterías)
    REG18_VOLTAGE            = 0x22,   // Regulador interno de 1.8V
    VSS_VOLTAGE              = 0x24,   // Medición de tierra usando ADC

    // Temperatura e indicadores térmicos
    INT_TEMPERATURE          = 0x28,   // Temperatura interna del BQ76905
    TS_MEASUREMENT           = 0x2A,   // Medición del pin de sensor térmico (TS)

    // Medición de corriente
    RAW_CURRENT              = 0x36,   // Medición cruda de corriente en 32 bits
    CURRENT_MEASUREMENT      = 0x3A,   // Medición de corriente CC2 en 16 bits
    CC1_CURRENT_MEASUREMENT  = 0x3C,   // Medición de corriente CC1 en 16 bits

    // Alarmas y control de salida
    ALARM_STATUS             = 0x62,   // Estado de alarmas activas
    ALARM_RAW_STATUS         = 0x64,   // Estado en tiempo real de alarmas
    REGOUT_CONTROL           = 0x69,   // Control de salida de regulador de voltaje
    DSG_FET_PWM_CONTROL      = 0x6A,   // Control PWM del FET de descarga
    CHG_FET_PWM_CONTROL      = 0x6C,   // Control PWM del FET de carga

    // **NUEVOS REGISTROS: SUBCOMANDOS**
    CELL_UNDERVOLTAGE_THRESHOLD = 0x902E, // Umbral de protección por bajo voltaje de celda
    CELL_OVERVOLTAGE_THRESHOLD  = 0x9032, // Umbral de protección por sobrevoltaje de celda
    OVERCURRENT_CHARGE_THRESHOLD = 0x9036, // Protección de sobrecorriente en carga
    OVERCURRENT_DISCHARGE_1      = 0x9038, // Protección de sobrecorriente en descarga 1
    OVERCURRENT_DISCHARGE_2      = 0x903A, // Protección de sobrecorriente en descarga 2
    SHORT_CIRCUIT_DISCHARGE      = 0x903C, // Protección de cortocircuito en descarga
    LATCH_LIMIT                  = 0x903E, // Límite de bloqueo de protección
    RECOVERY_TIME                = 0x903F, // Tiempo de recuperación tras una falla

    // Protección por temperatura
    OVERTEMP_CHARGE_THRESHOLD    = 0x9040, // Protección por sobretemperatura en carga
    UNDERTEMP_CHARGE_THRESHOLD   = 0x9043, // Protección por baja temperatura en carga
    OVERTEMP_DISCHARGE_THRESHOLD = 0x9046, // Protección por sobretemperatura en descarga
    UNDERTEMP_DISCHARGE_THRESHOLD = 0x9049, // Protección por baja temperatura en descarga
    INTERNAL_OVERTEMP_THRESHOLD  = 0x904C, // Protección por sobretemperatura interna

    // Configuración de Power Management
    SLEEP_CURRENT                = 0x904F, // Corriente en modo sleep
    SHUTDOWN_CELL_VOLTAGE        = 0x9053, // Voltaje de celda para apagado
    SHUTDOWN_STACK_VOLTAGE       = 0x9055, // Voltaje de la pila para apagado
    SHUTDOWN_TEMPERATURE         = 0x9057, // Temperatura de apagado
    AUTO_SHUTDOWN_TIME           = 0x9058, // Tiempo automático de apagado

    // Configuración de Seguridad
    SECURITY_SETTINGS            = 0x9059, // Configuración de seguridad
    FULL_ACCESS_KEY_1            = 0x905A, // Clave de acceso total - Paso 1
    FULL_ACCESS_KEY_2            = 0x905C  // Clave de acceso total - Paso 2

} BQ76905_Registers;

// Estructura para almacenar datos del BQ76905
typedef struct {
    I2C_HandleTypeDef *hi2c;  // Interfaz I2C utilizada

    // Voltajes de celdas
    uint16_t cell_voltages[5];   // Almacena voltajes de hasta 5 celdas

    // Voltajes generales
    uint16_t stack_voltage;      // Voltaje total del pack
    uint16_t reg18_voltage;      // Voltaje del regulador interno
    uint16_t vss_voltage;        // Medición de referencia de tierra

    // Temperatura
    int16_t internal_temperature; // Temperatura interna del BQ76905
    uint16_t ts_measurement;      // Medición del sensor térmico externo

    // Corriente
    int32_t raw_current;         // Medición de corriente cruda en 32 bits
    int16_t current;             // Medición de corriente CC2 en 16 bits
    int16_t cc1_current;         // Medición de corriente CC1 en 16 bits

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

} BQ76905_Device;


// Funciones para configurar y leer datos del BQ76905
HAL_StatusTypeDef BQ76905_WriteSubcommand(BQ76905_Device *bms, BQ76905_Registers subcmd);
HAL_StatusTypeDef BQ76905_WriteRegister(BQ76905_Device *bms, BQ76905_Registers reg, uint8_t *data, uint8_t len);
HAL_StatusTypeDef BQ76905_WriteChecksum(BQ76905_Device *bms, BQ76905_Registers reg, uint8_t *data, uint8_t len);
HAL_StatusTypeDef BQ76905_ReadRegister(BQ76905_Device *bms, BQ76905_Registers reg, uint8_t *rxData, uint8_t len);
void BQ76905_Configure(BQ76905_Device *bms);
void BQ76905_ReadData(BQ76905_Device *bms);





#ifdef __cplusplus
}
#endif

#endif /* INC_BMS_H_ */
