/*
 * pdu.c
 *
 *  Created on: Mar 2, 2025
 *      Author: apple
 */
#include "pdu.h"
#include "adc.h"
#include "usart.h"
#include "i2c.h"
#include <stdio.h>
#include <string.h>

#define VOLTAGE_STM32 3.3
#define ADC_RESOLUTION 4095.0 	//2^12 - 1


PDU_Channel PDU_Create(char *label, ADC_HandleTypeDef *hadc, uint32_t v_channel, uint32_t i_channel, GPIO_TypeDef *gpio_port, uint16_t gpio_pin) {
    PDU_Channel newPdu = {
        .hadc = hadc,
        .v_channel = v_channel,
        .i_channel = i_channel,
        .voltage = 0,
        .current = 0,
        .label = label,
        .gpio_port = gpio_port,
        .gpio_pin = gpio_pin
    };
    return newPdu;
}




void updatePDU(PDU_Channel *pdu) {
	//if (strcmp(pdu->label, "VBatOut") != 0)//Para que no se rompa el codigo, ya que bat out es una salida q no tiene con el adc

	// Leer valores de voltaje y corriente del ADC
	pdu->voltage = readADC(pdu->hadc, pdu->v_channel) * 2 * VOLTAGE_STM32 / ADC_RESOLUTION;
    HAL_Delay(2);  // 2 ms debería ser suficiente
	pdu->current = readADC(pdu->hadc, pdu->i_channel) * VOLTAGE_STM32 / ADC_RESOLUTION;
    HAL_Delay(2);  // 2 ms debería ser suficiente
}

void printPDUData(PDU_Channel *pdu) {
    char buffer[STR_LEN];
    snprintf(buffer, STR_LEN, "%s: %d V, %d A\n", pdu->label, pdu->voltage, pdu->current);
    HAL_I2C_Master_Transmit(&hi2c3, ARDUINO_I2C_ADDRESS << 1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}


// Habilitar la salida GPIO asociada a la fuente
void enablePDU(PDU_Channel *pdu) {
    HAL_GPIO_WritePin(pdu->gpio_port, pdu->gpio_pin, GPIO_PIN_SET);
}

// Deshabilitar la salida GPIO asociada a la fuente
void disablePDU(PDU_Channel *pdu) {
    HAL_GPIO_WritePin(pdu->gpio_port, pdu->gpio_pin, GPIO_PIN_RESET);
}
