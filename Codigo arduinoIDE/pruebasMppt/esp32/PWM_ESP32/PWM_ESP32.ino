#include "soc/soc_caps.h"

// Definición de los pines
#define PIN_PWM_1 32 // Pin para la señal PWM
#define PIN_PWM_2 23 // Pin para la señal PWM
#define PIN_PWM_3 21 // Pin para la señal PWM
#define PIN_PWM_4 13 // Pin para la señal PWM
#define LED_BUILTIN 2 // Definición del LED integrado en la placa
#define FRECUENCIA  200000 // 200KHz
#define RESOLUCION 8 // Resolución de 8 bits (0-255)
int duty1 = 32; // Duty cycle inicial del 50%
int duty2 = 64; // Duty cycle inicial del 50%
int duty3 = 96; // Duty cycle inicial del 50%
int duty4 = 128; // Duty cycle inicial del 50%


void setup() {
  pinMode(PIN_PWM_1, OUTPUT);
  pinMode(PIN_PWM_2, OUTPUT);
  pinMode(PIN_PWM_3, OUTPUT);
  pinMode(PIN_PWM_4, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Configuración del pin para PWM a 200KHz y resolución de 8 bits
  ledcAttach(PIN_PWM_1, FRECUENCIA, RESOLUCION);
  ledcAttach(PIN_PWM_2, FRECUENCIA, RESOLUCION);
  ledcAttach(PIN_PWM_3, FRECUENCIA, RESOLUCION);
  ledcAttach(PIN_PWM_4, FRECUENCIA, RESOLUCION);

  // Ajuste del duty cycle inicial (50%)
  ledcWrite(PIN_PWM_1, duty1);
  ledcWrite(PIN_PWM_2, duty2);
  ledcWrite(PIN_PWM_3, duty3);
  ledcWrite(PIN_PWM_4, duty4);

  // Encender el LED integrado
  digitalWrite(LED_BUILTIN, HIGH);

  Serial.begin(9600); // Para monitorear el duty cycle por el puerto serie
}

void loop() {
  // Monitoreo del duty cycle por el puerto serie
  Serial.print("Duty Cycle: ");
  Serial.println(duty1);
  
  // Incrementar el duty cycle, con reinicio al alcanzar el límite
  duty1 = (duty1 + 1) % 256;
  duty2 = (duty2 + 1) % 256;
  duty3 = (duty3 + 1) % 256;
  duty4 = (duty4 + 1) % 256;
  // Mantener el PWM en funcionamiento con el duty cycle establecido
  ledcWrite(PIN_PWM_1, duty1);
  ledcWrite(PIN_PWM_2, duty2);
  ledcWrite(PIN_PWM_3, duty3);
  ledcWrite(PIN_PWM_4, duty4);

  // Se puede ajustar el valor de "duty" aquí si se desea cambiar el ciclo de trabajo dinámicamente

  delay(1000); // Espera de 1 segundo
}

