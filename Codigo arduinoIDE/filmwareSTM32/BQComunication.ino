// Definir el pin donde se generará la señal PWM
const int pwmPin = PA8;           // Cambia por el pin que estés usando
const int pinVoltageInput = PA1;  // Entrada analógica para medir el voltaje de entrada A0
const int pinCurrentInput = PA6;  // Entrada analógica para medir la corriente de entrada A1

// Variables globales
float V_in = 0;
float I_in = 0;
float power = 0;
float previousPower = 0;
int dutyCycle = 255 * 0.5;  // Valor inicial del Duty Cycle (50% para PWM de 8 bits)
const int deltaDuty = 1;    // Incremento o decremento del Duty Cycle

void setup() {
  // Establecer la frecuencia del PWM en 62 kHz
  analogWriteFrequency(62 * 1000);  // 62 kHz
  pinMode(pwmPin, OUTPUT);

  Serial.begin(9600);  // Para monitoreo a través del puerto serie
}

void mppt(int *dutyCycle, float *power, float *previousPower) {
    // Algoritmo Perturbar y Observar
    if (*power > *previousPower) {
        if (*dutyCycle < 255) *dutyCycle += deltaDuty;  // Si la potencia ha aumentado, continuar ajustando en la misma dirección
    } else {
        if (*dutyCycle > 0) *dutyCycle -= deltaDuty;    // Si la potencia ha disminuido, invertir la dirección del ajuste
    }

    if (*dutyCycle < 0) *dutyCycle = 0;                 // Asegurar que el Duty Cycle esté dentro de los límites permitidos (0-255)
    if (*dutyCycle > 255) *dutyCycle = 255;          

    analogWrite(pwmPin, *dutyCycle);                    // Actualizar el PWM con el nuevo Duty Cycle
  
    *previousPower = *power;                            // Actualizar `previousPower` con el valor actual de `power`
}

void loop() {
  V_in = analogRead(pinVoltageInput) * (3.3 / 1023.0);  // Leer voltaje de entrada    // Conversión ADC a voltaje (suponiendo 3.3 V de referencia)
  I_in = analogRead(pinCurrentInput) * (3.3 / 1023.0);  // Leer corriente de entrada  // Conversión ADC a corriente

  power = V_in * I_in;                                  // Calcular potencia
  
  mppt(&dutyCycle, &power, &previousPower);
  
  //dutyCycle = 253;                                    //harcodeo el duty para probar    // Guardar el valor de potencia anterior
  previousPower = power;

  printf("Valor de Vin: %.2f, Valor de Iin: %.2f\n", V_in, I_in);
  printf("Potencia total: %.2f\n", power);

  delay(1000);
}
