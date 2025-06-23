
#include "bms.h"
#include "i2c.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>





// Función para configurar el BQ29330
void BQ29330_Configure(BQ29330_Device *bms) {
	// leer el manual para configurar el integrado

    char buffer[BUFFER_SIZE];

    // 🟢 Encabezado
    snprintf(buffer, BUFFER_SIZE, "\n--- Datos del BMS ---\n");
    HAL_I2C_Master_Transmit(&hi2c1, ARDUINO_I2C_ADDRESS << 1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
    HAL_Delay(10);


    // 🟢 Fin del mensaje
    snprintf(buffer, BUFFER_SIZE, "--------------------------\n");
    HAL_I2C_Master_Transmit(&hi2c1, ARDUINO_I2C_ADDRESS << 1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
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

void BQ29330_config(){
	BQ29330_WriteRegister(BQ29330_OUTPUT_CONTROL, 0x0E); 	//CHG =1 DCH =1 LTCLR=1
	BQ29330_WriteRegister(BQ29330_STATE_CONTROL, 4);   		// WDDIS = 1, all else = 0
	BQ29330_WriteRegister(BQ29330_FUNCTION_CONTROL, 0x05); 	// BAT = 1 y vmem 1
	BQ29330_WriteRegister(BQ29330_CELL, 0x01); 			   	// midiendo cell 1
    BQ29330_WriteRegister(BQ29330_OLV, 0x1F);              	// 50 mV 0x00
    BQ29330_WriteRegister(BQ29330_OLD, 0x0F);              	// 31 ms
    BQ29330_WriteRegister(BQ29330_SCC, 0xFF);              	// 475 mV y 915 μs
    BQ29330_WriteRegister(BQ29330_SCD, 0xFF);              	// idem for discharge

}



