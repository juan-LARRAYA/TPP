/*
 * mttp.h
 *
 *  Created on: Mar 2, 2025
 *      Author: apple
 */

#ifndef INC_MPPT_H_
#define INC_MPPT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"



// Estructura para encapsular la configuración de cada canal MPPT
typedef struct {
    ADC_HandleTypeDef *hadc;  // ADC usado para la lectura
    uint32_t v_channel;       // Canal ADC para voltaje
    uint32_t i_channel;       // Canal ADC para corriente
    TIM_HandleTypeDef *htim;  // Timer PWM asociado
    uint32_t tim_channel;     // Canal del Timer PWM
    uint16_t voltage;            // Valor de voltaje medido
    uint16_t current;            // Valor de corriente medido
    uint16_t power;              // Potencia calculada
    uint16_t prevPower;          // Potencia anterior para MPPT
    uint8_t dutyCycle;        // Duty Cycle del PWM
    uint8_t ultimaVariacion;
    char *label;        // Nombre para impresión (ej. "Eje_X", "Eje_y")
} MPPT_Channel;

// Declaración de la función MPPT
MPPT_Channel MPPT_Create(char *label, ADC_HandleTypeDef *hadc, uint32_t v_channel, uint32_t i_channel, TIM_HandleTypeDef *htim, uint32_t tim_channel);

//void mppt_algorithm(uint8_t *dutyCycle, const uint16_t *power, uint16_t *prevPower, uint8_t *ultimaVariacion);
void mppt_algorithm(uint8_t *dutyCycle, const uint16_t *power, uint16_t *prevPower, uint16_t * voltage);

void updateMPPT(MPPT_Channel *mppt);
void printMPPTData(MPPT_Channel *mppt);
void duty_limit(uint8_t *dutyCycle,uint8_t deltaDuty);


#ifdef __cplusplus
}
#endif


#endif /* INC_MPPT_H_ */
