#include "stdio.h"
#include "string.h"

// void HAL_TIM_SET_COMPARE(int dutyCycle);
// void mppt(int *dutyCycle, float *power, float *previousPower);

void HAL_TIM_SET_COMPARE(int dutyCycle){
    printf("magias \n");
}

void mppt(int *dutyCycle, float *power, float *previousPower) {
	const int deltaDuty = 1;    // Incremento o decremento del Duty Cycle
	if (*power > *previousPower) {
		if (*dutyCycle < 255)
			*dutyCycle += deltaDuty; // Si la potencia ha aumentado, continuar ajustando en la misma dirección
	} else {
		if (*dutyCycle > 0)
			*dutyCycle -= deltaDuty; // Si la potencia ha disminuido, invertir la dirección del ajuste
	}

	if (*dutyCycle < 0)
		*dutyCycle = 0; // Asegurar que el Duty Cycle esté dentro de los límites permitidos (0-255)
	if (*dutyCycle > 255)
		*dutyCycle = 255;

	HAL_TIM_SET_COMPARE(*dutyCycle); // Actualizar el PWM con el nuevo Duty Cycle

	*previousPower = *power; // Actualizar `previousPower` con el valor actual de `power`
}


int main(void){
	// Variables 
    float V_in = 0;
    float I_in = 0;
    float power = 0;
    float previousPower = 0;
    int dutyCycle = 255 * 0.5; // Valor inicial del Duty Cycle (50% para PWM de 8 bits)
    
    printf("%i \n", dutyCycle);    
    mppt(&dutyCycle, &power, &previousPower); //no anda no modifica el duty revisar
    printf("%i \n",dutyCycle);    

}