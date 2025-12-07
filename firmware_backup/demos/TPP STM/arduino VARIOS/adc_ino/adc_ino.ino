
int n = 0;

void setup(void) {
  Serial.begin(9600);

  uint32_t currentFrequency;

  Serial.println("Hello!");

}

void loop(void) {
  float adc1 = scaleValue(analogRead(A0));
  float adc2 = scaleValue(analogRead(A1));
  float Diference = adc2 - adc1;

  n++;
  if (n == 10) {
    Serial.print("ADC 1: ");
    Serial.print(adc1);
    Serial.println(" mV");

    Serial.print("ADC 2: ");
    Serial.print(adc2);
    Serial.println(" mV");
    
    Serial.println(" ");
    
    Serial.print("Diference: ");
    Serial.print(Diference);
    Serial.println(" mV");



    Serial.println(" ");
    n = 0;
  }


  delay(200);
}


// Función de mapeo para flotantes
float scaleValue(float x) {
  return (x * 4.88);  // 5.0/1.023);
                      //(975 - x * 5.0/1.023)/0.15;
}
