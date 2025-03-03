/*
 * bms.c
 *
 *  Created on: Mar 2, 2025
 *      Author: apple
 */

#include "bms.h"
#include "i2c.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>

// Función para escribir en un registro del BQ76905
HAL_StatusTypeDef BQ76905_WriteRegister(uint8_t reg, uint8_t* data, uint16_t len) {
    uint8_t buffer[len + 1];
    buffer[0] = reg;
    memcpy(&buffer[1], data, len);

    return HAL_I2C_Master_Transmit(&hi2c3, BQ76905_I2C_ADDR, buffer, len + 1, HAL_MAX_DELAY);
}

// Función para leer un registro específico del BQ76905
HAL_StatusTypeDef BQ76905_ReadRegister(uint8_t reg, uint8_t* data, uint16_t len) {
    HAL_StatusTypeDef status;

    status = HAL_I2C_Master_Transmit(&hi2c3, BQ76905_I2C_ADDR, &reg, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return status;
    }

    return HAL_I2C_Master_Receive(&hi2c3, BQ76905_I2C_ADDR, data, len, HAL_MAX_DELAY);
}

// Función para leer un registro usando HAL_I2C_Mem_Read
HAL_StatusTypeDef BQ76905_ReadRegister_test(uint8_t regAddr, uint8_t *data, uint16_t size) {
    return HAL_I2C_Mem_Read(&hi2c3, BQ76905_I2C_ADDR, regAddr, I2C_MEMADD_SIZE_8BIT, data, size, HAL_MAX_DELAY);
}

// Función para leer el voltaje de una celda
void ReadCellVoltage(I2C_HandleTypeDef *hi2c, uint8_t cell) {
    uint8_t cmd_addr = 0x14 + (cell * 2);
    uint8_t result[2] = {0};  // Array para almacenar los datos de lectura
    char buffer[100];

    if (BQ76905_ReadRegister_test(cmd_addr, result, 2) == HAL_OK) {
        uint16_t voltage = (result[0] | (result[1] << 8)); // Convertir los bytes a un valor de voltaje
        sprintf(buffer, "Celda %d = %d mV\n", cell, voltage);
        HAL_UART_Transmit(&huart4, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
    } else {
        char MSG_READ_ERROR[] = "\n Error al leer \n";
        HAL_UART_Transmit(&huart4, (uint8_t*)MSG_READ_ERROR, sizeof(MSG_READ_ERROR) - 1, HAL_MAX_DELAY);
    }
}

// Función para habilitar el FET de descarga
void enable_Discharge_FET() {
    uint8_t command[2] = {FET_CONTROL_CMD, 0x01};  // 0x01 para habilitar descarga
    HAL_I2C_Master_Transmit(&hi2c3, BQ76905_I2C_ADDR, command, 2, HAL_MAX_DELAY);
}

// Función para deshabilitar el FET de descarga
void disable_Discharge_FET() {
    uint8_t command[2] = {FET_CONTROL_CMD, 0x00};  // 0x00 para deshabilitar descarga
    HAL_I2C_Master_Transmit(&hi2c3, BQ76905_I2C_ADDR, command, 2, HAL_MAX_DELAY);
}
