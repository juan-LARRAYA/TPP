
#include "bms.h"
#include "i2c.h"
#include "usart.h"





// Función para configurar el BQ29330
void BQ29330_Configure(BQ29330_Device *bms) {
	// leer el manual para configurar el integrado

}


void sendBMSDataI2C(BQ29330_Device *bms) {
    char buffer[BUFFER_SIZE];

    // 🟢 Encabezado
    snprintf(buffer, BUFFER_SIZE, "\n--- Datos del BMS ---\n");
    HAL_I2C_Master_Transmit(&hi2c1, ARDUINO_I2C_ADDRESS << 1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
    HAL_Delay(10);


    // 🟢 Fin del mensaje
    snprintf(buffer, BUFFER_SIZE, "--------------------------\n");
    HAL_I2C_Master_Transmit(&hi2c1, ARDUINO_I2C_ADDRESS << 1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}


void INA219_Init(void)
{
    // Aquí podrías configurar registros de calibración si fuera necesario
    // Por defecto, se asume calibración 32V, 2A
}

uint16_t INA219_ReadBusVoltage(void)
{
    uint8_t buffer[2];
    uint16_t raw;

    HAL_I2C_Mem_Read(&hi2c2, INA219_ADDRESS, INA219_REG_BUS_VOLTAGE, 1, buffer, 2, HAL_MAX_DELAY);
    raw = (buffer[0] << 8) | buffer[1];
    raw >>= 3;  // los 3 bits LSB no son válidos
    return raw * 0.004f;  // resolución 4 mV
}

uint16_t INA219_ReadShuntVoltage(void)
{
    uint8_t buffer[2];
    int16_t raw;

    HAL_I2C_Mem_Read(&hi2c2, INA219_ADDRESS, INA219_REG_SHUNT_VOLTAGE, 1, buffer, 2, HAL_MAX_DELAY);
    raw = (int16_t)((buffer[0] << 8) | buffer[1]);
    return raw * 0.01f;  // resolución 10 uV
}

uint16_t INA219_ReadCurrent(void)
{
    uint8_t buffer[2];
    int16_t raw;

    HAL_I2C_Mem_Read(&hi2c2, INA219_ADDRESS, INA219_REG_CURRENT, 1, buffer, 2, HAL_MAX_DELAY);
    raw = (int16_t)((buffer[0] << 8) | buffer[1]);
    return raw * 1.0f;  // depende de la calibración
}

uint16_t INA219_ReadPower(void)
{
    uint8_t buffer[2];
    uint16_t raw;

    HAL_I2C_Mem_Read(&hi2c2, INA219_ADDRESS, INA219_REG_POWER, 1, buffer, 2, HAL_MAX_DELAY);
    raw = (buffer[0] << 8) | buffer[1];
    return raw * 20.0f;  // resolución depende de calibración: 20 mW/LSB para default
}


// Función para leer un registro
HAL_StatusTypeDef BQ29330_ReadRegister(BQ29330_Registers reg, uint8_t *rxData) {
    HAL_Delay(50);
    HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(&hi2c1, BMS_I2C_ADDRESS, (uint8_t*) & reg, 1, HAL_MAX_DELAY);
    if (ret != HAL_OK) return ret;
    return HAL_I2C_Master_Receive(&hi2c1, BMS_I2C_ADDRESS, rxData, 1, HAL_MAX_DELAY);
}




// Lee el valor actual del registro FUNCTION_CONTROL (0x03) del BQ29330
HAL_StatusTypeDef BQ29330_ReadFunctionControl(BQ29330_Registers reg, uint8_t *valor_leido)
{
	uint8_t buffer[1];
    buffer[0] = reg; 		// Dirección del registro
    // Paso 1: Enviar la dirección del registro que queremos leer (0x03)
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, BMS_I2C_ADDRESS, buffer, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) return status;  // Si falla al enviar la dirección del registro, retornamos el error
    // Paso 2: Leer un byte desde el dispositivo (valor del registro FUNCTION_CONTROL)
    return HAL_I2C_Master_Receive(&hi2c1, BMS_I2C_ADDRESS, valor_leido, 1, HAL_MAX_DELAY);  // Devolver el estado de la operación y el valor leído en *valor_leido
}



// Función para escribir en un registro del BQ29330
HAL_StatusTypeDef BQ29330_WriteRegister(BQ29330_Registers reg, uint8_t data)
{
    uint8_t buffer[2];
    buffer[0] = reg; 		// Dirección del registro
    buffer[1] = data;        // Dato a escribir
    return HAL_I2C_Master_Transmit(&hi2c1, BMS_I2C_ADDRESS, buffer, 2, HAL_MAX_DELAY);
}



