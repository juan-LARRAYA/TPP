/*
 * pdu.c
 *
 *  Created on: Mar 2, 2025
 *      Author: apple
 */
#include "pdu.h"
#include "adc.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>


void updatePDU(PDU_Channel *pdu) {
    // Leer valores de voltaje y corriente del ADC
    pdu->voltage = readADC(pdu->hadc, pdu->v_channel);
    pdu->current = readADC(pdu->hadc, pdu->i_channel);

    // Enviar datos por UART
    char buffer[STR_LEN];
    snprintf(buffer, STR_LEN, "%s: %.2f V, %.2f A\n", pdu->label, pdu->voltage, pdu->current);
    HAL_UART_Transmit(&huart4, (uint8_t*) buffer, strlen(buffer), HAL_MAX_DELAY);
}

// Habilitar la salida GPIO asociada a la fuente
void enablePDU(PDU_Channel *pdu) {
    HAL_GPIO_WritePin(pdu->gpio_port, pdu->gpio_pin, GPIO_PIN_SET);
}

// Deshabilitar la salida GPIO asociada a la fuente
void disablePDU(PDU_Channel *pdu) {
    HAL_GPIO_WritePin(pdu->gpio_port, pdu->gpio_pin, GPIO_PIN_RESET);
}
