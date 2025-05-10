void setup() {
  // Configura el pin PA5 como salida
  pinMode(PC13, OUTPUT);
  pinMode(PC15, OUTPUT);
  digitalWrite(PC15, HIGH);
<<<<<<< HEAD
  Serial.begin(9600);
=======
>>>>>>> 929fc7b3691383351f171012fbc5cb99b2508140
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
<<<<<<< HEAD

  Serial.println("working!");
=======
>>>>>>> 929fc7b3691383351f171012fbc5cb99b2508140
}


