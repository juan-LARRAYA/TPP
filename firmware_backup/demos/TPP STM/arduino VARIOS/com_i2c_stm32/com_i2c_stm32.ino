#include <Wire.h>

#define I2C_ADDRESS 0x08  // Dirección I2C del Arduino esclavo
#define LED_PIN 13        // Pin del LED integrado
int i = 0;

void setup() {
  Serial.begin(9600);  // Baudrate más alto para depuración
  pinMode(LED_PIN, OUTPUT);
  Wire.begin(I2C_ADDRESS);       // Inicia el esclavo I2C con dirección 0x08
  Wire.onReceive(receiveEvent);  // Función llamada al recibir datos
  Wire.onRequest(requestEvent);  // Función llamada al recibir una solicitud
}

void loop() {
  i = 0;
  digitalWrite(LED_PIN, HIGH);
  delay(2000);
  digitalWrite(LED_PIN, LOW);
  delay(2000);
}

// 🟢 Función para recibir datos desde el STM32
void receiveEvent(int numBytes) {
  //Serial.print("Mensaje #: ");
  //Serial.println(i++);
  //Serial.print("Dato recibido: ");
  while (Wire.available()) {
    char receivedChar = Wire.read();
    Serial.write(receivedChar);
    //Serial.println(receivedChar,BIN);           // Imprime en hexadecimal
  }
}

// 🔵 Función para enviar datos al STM32 cuando lo solicite
void requestEvent() {
  const char mensaje[] = "Hola STM32";       // Mensaje fijo
  Wire.write(mensaje, sizeof(mensaje) - 1);  // Envía el mensaje sin el terminador '\0'
}

void configurarPWM_400kHz() {
  // Configurar Timer1 en modo Fast PWM, sin prescaler
  TCCR1A = (1 << COM1A0);               // Toggle OC1A (Pin 9) en cada comparación
  TCCR1B = (1 << WGM12) | (1 << CS10);  // CTC Mode, prescaler = 1
  OCR1A = 19;                           // (16MHz / (2 * (OCR1A + 1))) = 400kHz
}
212223242526272829303132333435363738394041424344454647
}

// 🟢 Función para recibir datos desde el STM32
void receiveEvent(int numBytes) {
  //Serial.print("Mensaje #: ");
  //Serial.println(i++);
  //Serial.print("Dato recibido: ");
  while (Wire.available()) {
    char receivedChar = Wire.read();
    Serial.write(receivedChar);

Message (Enter to send message to 'Arduino Uno' on '/dev/cu.usbserial-130')
New Line
9600 baud

