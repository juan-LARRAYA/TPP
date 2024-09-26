#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // Inicializa la comunicación serial a 115200 baudios
  Serial.begin(115200);

  // Configura el pin PA5 como salida
  pinMode(PA5, OUTPUT);
  
  // Poner el pin PA5 en alto (1)
  digitalWrite(PA5, HIGH);

}

void loop() {
  // No es necesario hacer nada en el loop
  int result = myFunction(2, 3);
  // Imprime el número entero por el puerto serial
  Serial.print("El número es: ");
  Serial.println(result);
}



// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

