// Definir los pines
const int pinPWM = 11;            // Salida PWM para controlar el MOSFET
const int pinVoltageInput = A0;  // Entrada analógica para medir el voltaje de entrada
const int pinCurrentInput = A1;  // Entrada analógica para medir la corriente de entrada

// Variables globales
float V_in = 0;
float I_in = 0;
float Power = 0;
float previousPower = 0;
int dutyCycle = 205;      // Valor inicial del Duty Cycle (50% para PWM de 8 bits)
const int deltaDuty = 1;  // Incremento o decremento del Duty Cycle

void setup() {
  pinMode(pinPWM, OUTPUT);
  //analogWrite(pinPWM, dutyCycle); // Configurar el PWM inicial 
  Serial.begin(9600);  // Para monitoreo a través del puerto serie

  // Configurar el Timer1 para PWM en modo Fast PWM (16 bits)
  TCCR1A = (1 << COM1A1) | (1 << WGM11);               // Clear OC1A on Compare Match, modo Fast PWM
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12);  // Prescaler de 1, modo Fast PWM
  ICR1 = 39999;                                        // Frecuencia de PWM a 62.5 kHz (16 MHz / (1 * (256)))

  // Configurar el ADC
  ADMUX = (1 << REFS0);                                // Usar AVcc como referencia de voltaje
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);  // Habilitar ADC, prescaler de 64 (16 MHz / 64 = 250 kHz)
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
  analogWrite(pinPWM, dutyCycle);
  //OCR1A = dutyCycle;

  // Guardar el valor de potencia anterior
  previousPower = Power;

  // Salida de información para monitoreo
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
  Serial.println(" %");

  // Esperar un tiempo antes de la próxima iteración
  delay(100);
}
