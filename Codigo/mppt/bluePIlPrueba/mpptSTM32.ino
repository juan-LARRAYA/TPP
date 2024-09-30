// Definir el pin donde se generará la señal PWM
const int pwmPin = PA8;  // Cambia por el pin que estés usando
const int pinVoltageInput = PA0;  // Entrada analógica para medir el voltaje de entrada A0
const int pinCurrentInput = PA1;  // Entrada analógica para medir la corriente de entrada A1

// Variables globales
float V_in = 0;
float I_in = 0;
float Power = 0;
float previousPower = 0;
int dutyCycle = 205;      // Valor inicial del Duty Cycle (50% para PWM de 8 bits)
const int deltaDuty = 1;  // Incremento o decremento del Duty Cycle

void setup() {
  // Configurar el pin como salida de PWM
  // pinMode(pwmPin, OUTPUT);
  pinMode(pwmPin, PWM); 
  //Serial.begin(9600);  // Para monitoreo a través del puerto serie

}

void loop() {
  // Leer voltaje de entrada
  V_in = analogRead(pinVoltageInput) * (5.0 / 1023.0);  // Conversión ADC a voltaje (suponiendo 5V de referencia)

  // Leer corriente de entrada
  I_in = analogRead(pinCurrentInput) * (5.0 / 1023.0);  // Conversión ADC a corriente

  // Calcular potencia
  Power = V_in * I_in;
  /*// Algoritmo Perturbar y Observar (COMENTAR ESTE CODIGO PARA PODER FIJAR EN DUTY EN EL NUMERO QUE PUSIMOS ARRIBA
  if (Power > previousPower) {
    // Si la potencia ha aumentado, continuar ajustando en la misma dirección
    if (dutyCycle < 255) dutyCycle += deltaDuty;
  } else {
    // Si la potencia ha disminuido, invertir la dirección del ajuste
    if (dutyCycle > 0) dutyCycle -= deltaDuty;
  }*/

  // Asegurar que el Duty Cycle esté dentro de los límites permitidos (0-255)
  dutyCycle = constrain(dutyCycle, 0, 255);

  // Actualizar el PWM con el nuevo Duty Cycle
  analogWrite(pwmPin, dutyCycle);

  // Guardar el valor de potencia anterior
  previousPower = Power;
 

  /*// Salida de información para monitoreo
  Serial.print("Voltaje: ");
  Serial.print(V_in);
  Serial.print(" V ");
  Serial.print("Corriente: ");
  Serial.print(I_in);
  Serial.print(" A ");
  Serial.print("Potencia: ");
  Serial.print(Power);
  Serial.print(" W ");
  Serial.print("Duty Cycle: ");
  Serial.print((dutyCycle / 255.0) * 100);
  Serial.println(" %"); */

  delay(1000);
  
}
