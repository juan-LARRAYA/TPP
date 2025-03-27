/*
 * pdu.h
 *
 *  Created on: Mar 2, 2025
 *      Author: apple
 */

#ifndef INC_PDU_H_
#define INC_PDU_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

// Estructura para almacenar datos de voltaje, corriente y control de salida
typedef struct {
    ADC_HandleTypeDef *hadc;  // ADC usado para la lectura
    uint32_t v_channel;       // Canal ADC para voltaje
    uint32_t i_channel;       // Canal ADC para corriente
    uint16_t voltage;            // Última tensión medida
    uint16_t current;            // Última corriente medida
    const char *label;        // Nombre para impresión (ej. "V5", "V3bis")

    GPIO_TypeDef *gpio_port;  // Puerto GPIO para salida de habilitación
    uint16_t gpio_pin;        // Pin GPIO para salida de habilitación
} PDU_Channel;

// Funciones
PDU_Channel PDU_Create(char *label, ADC_HandleTypeDef *hadc, uint32_t v_channel, uint32_t i_channel, GPIO_TypeDef *gpio_port, uint16_t gpio_pin);
void updatePDU(PDU_Channel *pdu);
void printPDUData(PDU_Channel *pdu);
void enablePDU(PDU_Channel *pdu);
void disablePDU(PDU_Channel *pdu);


#ifdef __cplusplus
}
#endif


#endif /* INC_PDU_H_ */
