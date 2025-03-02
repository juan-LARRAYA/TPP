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
#include <stdint.h>


// Estructura para encapsular la configuración de cada canal MPPT
typedef struct {
    ADC_HandleTypeDef *hadc;  // ADC usado para la lectura
    uint32_t v_channel;       // Canal ADC para voltaje
    uint32_t i_channel;       // Canal ADC para corriente
    TIM_HandleTypeDef *htim;  // Timer PWM asociado
    uint32_t tim_channel;     // Canal del Timer PWM
    float voltage;            // Valor de voltaje medido
    float current;            // Valor de corriente medido
    float power;              // Potencia calculada
    float prevPower;          // Potencia anterior para MPPT
    uint8_t dutyCycle;        // Duty Cycle del PWM
} MPPT_Channel;

// Declaración de la función MPPT
void mppt_algorithm(uint8_t *dutyCycle, const float *power, float *prevPower);
void updateMPPT(MPPT_Channel *mppt);


#ifdef __cplusplus
}
#endif


#endif /* INC_MPPT_H_ */
