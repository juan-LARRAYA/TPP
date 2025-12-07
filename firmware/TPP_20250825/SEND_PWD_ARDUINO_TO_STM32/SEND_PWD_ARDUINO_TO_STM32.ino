
#include <Wire.h>

void setup() {
  Wire.begin(); // Inicializa I2C como maestro
  Serial.begin(115200);
  Serial.println("Arduino I2C Master Listo.");
}

void loop() {
  int stmI2cAddress = 17; // La dirección del esclavo STM32 es 17 (0x11)
  int dataToSend1 = 240; // El valor que quieres enviar
  int dataToSend2 = 10; // El valor que quieres enviar
  int dataToSend3 = 110; // El valor que quieres enviar
  
  // Inicia la transmisión a la dirección I2C del STM32
  Wire.beginTransmission(stmI2cAddress);
  
  // Escribe el byte a enviar
  Wire.write(dataToSend1);
  Wire.write(dataToSend2);
  Wire.write(dataToSend3);
  
  // Termina la transmisión
  Wire.endTransmission();
  
  Serial.print("Dato enviado al STM32: ");
  Serial.println(dataToSend1);
  Serial.println(dataToSend2);
  Serial.println(dataToSend3);

  // Espera 1 segundo antes de enviar de nuevo
  delay(1000);
}
