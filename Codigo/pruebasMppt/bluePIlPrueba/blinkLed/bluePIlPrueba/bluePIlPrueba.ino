void setup() {
  // Configura el pin PA5 como salida
  pinMode(PC13, OUTPUT);
  pinMode(PC15, OUTPUT);
  digitalWrite(PC15, HIGH);
  Serial.begin(9600);
}

void loop() {
  // No es necesario hacer nada en el loop
  // Poner el pin PA5 en alto (1)
  digitalWrite(PC13, HIGH);
  // Poner el pin PA5 en alto (1)
  delay(1000);
  digitalWrite(PC13, LOW);
  // Poner el pin PA5 en alto (1)
  delay(1000);

  Serial.println("working!");
}


