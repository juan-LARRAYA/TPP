#include "driver/ledc.h"

// Definición de los pines y parámetros
#define PIN_PWM 32 // Pin para la señal PWM
#define LED_BUILTIN 2 // Definición del LED integrado en la placa
#define FRECUENCIA  200000 // 20 MHz
#define RESOLUCION 8 // Resolución de 8 bits (0-255)
int duty = 128; // Duty cycle inicial del 50%

void setup() {
  pinMode(PIN_PWM, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Configuración del temporizador LEDC
  ledcSetup(0, FRECUENCIA, RESOLUCION);

  // Asociar el canal de PWM al pin especificado
  ledcAttachPin(PIN_PWM, 0);

  // Ajuste del duty cycle inicial (50%)
  ledcWrite(0, duty);

  // Encender el LED integrado
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(9600); // Para monitorear el duty cycle por el puerto serie
}

void loop() {
  // Monitoreo del duty cycle por el puerto serie
  Serial.print("Duty Cycle: ");
  Serial.println(duty);

  // Mantener el PWM en funcionamiento con el duty cycle establecido
  ledcWrite(0, duty);

  // Se puede ajustar el valor de "duty" aquí si se desea cambiar el ciclo de trabajo dinámicamente

  delay(1000); // Espera de 1 segundo
}

