
const int pinPWM = 9;            // Salida PWM para controlar el MOSFET
const int pinVoltageInput = A0;  // Entrada analógica para medir el voltaje de entrada
const int pinCurrentInput = A1;  // Entrada analógica para medir la corriente de entrada

// Variables globales
float V_in = 0;
float I_in = 0;
float Power = 0;
float previousPower = 255;

int dutyCycle = 18*255/100;      // Valor inicial del Duty Cycle (50% para PWM de 8 bits)

const int deltaDuty = 1;  // Incremento o decremento del Duty Cycle


void setup() {
  // Configurar el Timer 1 en modo Fast PWM con TOP = 257
  pinMode(pinPWM, OUTPUT);   // Configurar el pin 9 como salida
  
  Serial.begin(9600);  // Para monitoreo a través del puerto serie
  
  // Deshabilitar el Timer 1 para configurarlo
  TCCR1A = 0;           // Limpiar el registro de control A
  TCCR1B = 0;           // Limpiar el registro de control B
  
  // Configurar el modo Fast PWM y usar ICR1 como TOP
  TCCR1B |= (1 << WGM13) | (1 << WGM12);
  TCCR1A |= (1 << WGM11) | (1 << COM1A1); // Habilitar salida en pin 9 (OC1A)
  
  // Establecer el valor de TOP en ICR1 para una frecuencia de 62 kHz
  ICR1 = 257;
  
  // Establecer el valor de comparación (duty cycle, por ejemplo 50%)
  OCR1A = 128;  // 50% del ciclo de trabajo
  
  // Configurar el prescaler en 1
  TCCR1B |= (1 << CS10);  // Sin prescaler
}

void loop() {
  V_in = analogRead(pinVoltageInput) * (5.0 / 1023.0);  // Conversión ADC a voltaje (suponiendo 5V de referencia)
  I_in = analogRead(pinCurrentInput) * (5.0 / 1023.0);  // Conversión ADC a corriente
/*
  // Calcular potencia
  Power = V_in * I_in;
  // Algoritmo Perturbar y Observar (COMENTAR ESTE CODIGO PARA PODER FIJAR EN DUTY EN EL NUMERO QUE PUSIMOS ARRIBA
  if (Power > previousPower) {
    // Si la potencia ha aumentado, continuar ajustando en la misma dirección
    if (dutyCycle < 255) dutyCycle += deltaDuty;
  } else {
    // Si la potencia ha disminuido, invertir la dirección del ajuste
    if (dutyCycle > 0) dutyCycle -= deltaDuty;
  }
  */

  dutyCycle = constrain(dutyCycle, 0, 255);

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

  delay(100);
}

