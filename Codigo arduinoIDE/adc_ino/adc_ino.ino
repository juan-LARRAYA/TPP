
void setup() {
  Serial.begin(9600); // Comunicación serial para monitorear
}

void loop() {

  float adc1Scaled = analogRead(A0);
  float adc2Scaled = analogRead(A1);
  // Mostrar resultados
  Serial.print("ADC1: ");
  Serial.println(adc1Scaled);
  Serial.print(" ADC2: ");
  Serial.println(adc2Scaled);

  float Diference = scaleValue(adc2Scaled - adc1Scaled);
   Serial.print("Diference: ");
  Serial.println(Diference);
  delay(500); // Espera 500 ms entre lecturas
}

// Función de mapeo para flotantes
float scaleValue(float x) {
   return (975 - x * 5.0/1.023)/0.15;
}

