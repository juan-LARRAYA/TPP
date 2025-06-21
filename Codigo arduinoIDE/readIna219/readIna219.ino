#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;


volatile double corrienteInstantanea = 0;        // Se actualiza en loop
volatile double totalizadoCoulomb = 0;         // Acumulador de carga
int n = 0;

void setup(void) {
  Serial.begin(9600);
  noInterrupts();           // Desactivar interrupciones globales

  TCCR1A = 0;               // Timer en modo normal
  TCCR1B = 0;
  TCNT1  = 0;               // Reiniciar contador

  OCR1A = 24999;            // Comparación cada 100 ms
  TCCR1B |= (1 << WGM12);   // Modo CTC (Clear Timer on Compare Match)
  TCCR1B |= (1 << CS11) | (1 << CS10);  // Prescaler 64
  TIMSK1 |= (1 << OCIE1A);  // Habilitar interrupción por comparación con OCR1A

  interrupts();             // Activar interrupciones globales


  uint32_t currentFrequency;

  Serial.println("Hello!");

  // Initialize the INA219.
  if (!ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }

  // 16V, 400mA range (higher precision on volts and amps):
  ina219.setCalibration_16V_400mA();
  Serial.println("Measuring voltage and current with INA219 ...");
  
}

// 🧠 Esta es la función que se ejecuta cada 100 ms
ISR(TIMER1_COMPA_vect) {
  totalizadoCoulomb += corrienteInstantanea /10000;
}


void loop(void) {
  float adc1 = scaleValue(analogRead(A0));
  float adc2 = scaleValue(analogRead(A1));
  float Diference = scaleValue(adc1 - adc2);
 

  float shuntvoltage = 0;
  float current_mA = 0;
  float scale = 2.65;

  shuntvoltage = scale * ina219.getShuntVoltage_mV();
  current_mA = scale * ina219.getCurrent_mA();

  corrienteInstantanea = current_mA; // Actualiza para la ISR
  n++;
  if(n == 10){

    Serial.print("Shunt Voltage: ");
    Serial.print(shuntvoltage);
    Serial.println(" mV");

    Serial.print("Current: ");
    Serial.print(current_mA);
    Serial.println(" mA");

    Serial.print("Carga acumulada: ");
    Serial.print(scale * totalizadoCoulomb);
    Serial.println(" cC");

    Serial.println(" ");
    n = 0;
  }


  delay(200);
}


// Función de mapeo para flotantes
float scaleValue(float x) {
   return (975 - x * 5.0/1.023)/0.15;
}

