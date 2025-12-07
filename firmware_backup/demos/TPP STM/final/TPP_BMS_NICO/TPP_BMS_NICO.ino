#include <Wire.h>
#include <Adafruit_INA219.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

// Objetos y constantes
Adafruit_INA219 ina219BMS(0x40);

const int VOLTAGE_ENABLE_PIN            = 7;
const int ADC_DIVISOR_PIN               = A0;  // Entrada del divisor

/********** Pines de control*********************/
const uint8_t CHARGE_ENABLE_PIN         = 8;
const uint8_t DISCHARGE_ENABLE_PIN      = 9;

// ==== Balanceo por comando
const uint8_t BAL1_PIN                  = 10;   // Celda 1
const uint8_t BAL2_PIN                  = 11;   // Celda 2

/************CONSTANTES*************************************/
// Umbrales de protección (V)
const float OVERVOLTAGE_LIMIT           = 4.20;
const float UNDERVOLTAGE_LIMIT          = 3.30;
const float HYSTERESIS                  = 0.05; // 50 mV de margen para evitar oscilación
const float OVERCURRENT_IN_CHARGE       = 1200.0;
const float OVERCURRENT_IN_DISCHARGE    = -1200.0;
const float SHORTCIRCUIT_IN_CHARGE      = 1400.0;
const float SHORTCIRCUIT_IN_DISCHARGE   = -1400.0;
const float BALANCE_DELTA_V             = 0.08; // 80 mV
const float BALANCE_MIN_CELL_V          = 3.8;
//Constantes de tiempo
const uint8_t HICCUPTIME                = 20;
const uint8_t OCTIME                    = 20;
const uint8_t VOLTAGE_SAMPLE_TIME       = 50;

// Configuración ADC
const float REFERENCIA_INTERNA          = 1.1;    // 1.1V interna
const float DIVISOR_FACTOR              = 4.394;   // Factor del divisor resistivo

/***************Variables globales**********************************************/

//Carga-descarga
bool chargeEnabled    = true;
bool dischargeEnabled = true;

//Balance
bool balance1_request = false;  // pedido del usuario por serial
bool balance2_request = false;

// Control de corriente FLAGS:
bool dischargeOC = false;
bool chargeOC = false;
bool dischargeSC = false;
bool chargeSC = false;

// Control de corriente acumuladores:
uint8_t overcurrentChargeCounter = 0;
uint8_t overcurrentDischargeCounter = 0;

// Control de tension FLAGS:
bool cell1UV = false;
bool cell2UV = false;
bool cell1OV = false;
bool cell2OV = false;

//Control de flujo
volatile bool medirCorriente = false;
volatile bool medirVoltajes  = false;

//Control de tiempo
volatile unsigned int tickCounter = 0;
unsigned long lastMeasureTime = 0;

//Variables del sistema
float corriente     = 0.0;
float tensionINA    = 0.0;
float tensionADC    = 0.0;
float tensionB1    = 0.0;
float tensionB2    = 0.0;
double coulombCount = 0.0;   // mA·s acumulados



/************************************************************************************/
/******************************Initial SETUP*****************************************/
void setup() {
  Serial.begin(115200);
  Wire.begin();

  while (!ina219BMS.begin()) {
    Serial.println("Error al iniciar ina219BMS");
  }
  //ina219BMS.setCalibration_32V_2A();
  Serial.println("ina219BMS OK");

  pinMode(VOLTAGE_ENABLE_PIN, OUTPUT);
  digitalWrite(VOLTAGE_ENABLE_PIN, LOW);

  // Configurar referencia ADC a 1.1V interna
  analogReference(INTERNAL);

  setupTimer1();
  setupProtection();
  setupBalanceo();

  set_sleep_mode(SLEEP_MODE_IDLE);  // Modo IDLE: deja funcionando el timer y el I2C
  sei();  // Habilita interrupciones globales

  lastMeasureTime = millis();
}

/*********************************************************************/
/*****************MAIN LOOP¨****************************************/
void loop() {
  sleep_mode();  // El micro duerme hasta que el Timer1 lo despierte

  // --- Se ejecuta al despertar ---

  while (Serial.available() > 0) {
    char c = Serial.read();
    switch (c) {
      case 'a': balance1_request = true;  break;   // balance ON celda 1
      case 'A': balance1_request = false; break;   // balance OFF celda 1
      case 'b': balance2_request = true;  break;   // balance ON celda 2
      case 'B': balance2_request = false; break;   // balance OFF celda 2
      case 'x': balance1_request = false; balance2_request = false; break; // OFF ambos
      case 's': Serial.println(F("CMD s: status solicitado")); break;
    }
    Serial.print(F(" B1:")); Serial.println(balance1_request);
    Serial.print(F(" B2:")); Serial.println(balance2_request);
  }

  if (medirCorriente) {
    // Tiempo transcurrido desde última medición (en segundos)
    unsigned long now = millis();
    double deltaT_s = (now - lastMeasureTime) / 1000.0;
    lastMeasureTime = now;

    corriente = ina219BMS.getCurrent_mA();
    checkCellChargeCurrentProtection(corriente);
    checkCellDischargeCurrentProtection(corriente);
    applyProteccion();
    // Coulomb counting: corriente[mA] * tiempo[s]
    coulombCount += corriente * deltaT_s;
    medirCorriente = false;
  }

  if (medirVoltajes) {


    // Medir tensión con ina219BMS
    Serial.print("Corriente (mA): ");
    Serial.println(corriente);

    Serial.print("Carga acumulada (mAh): ");
    Serial.println(coulombCount / 3600.0, 5);

    tensionINA = ina219BMS.getBusVoltage_V();
    Serial.print("Tensión ina219BMS (V): ");
    Serial.println(tensionINA);

    // Activar divisor resistivo
    digitalWrite(VOLTAGE_ENABLE_PIN, HIGH);
    
    if(digitalRead(BAL1_PIN)){ //Desactivar balanceos para medir
      digitalWrite(BAL1_PIN , LOW);
    }
    if(digitalRead(BAL2_PIN)){
      digitalWrite(BAL2_PIN , LOW);
    }
    delay(20);  // Esperar estabilización mínima
    int adcDiv = analogRead(ADC_DIVISOR_PIN);
    digitalWrite(VOLTAGE_ENABLE_PIN, LOW);

    // Calcular tensión medida
    tensionADC = ((float)adcDiv / 1023.0) * REFERENCIA_INTERNA;
    
    tensionB1 = DIVISOR_FACTOR * tensionADC;
    if (tensionB1 < 0) tensionB1 = 0; //idk pero por las dudas
    tensionB2 = tensionINA - tensionB1;
    if(tensionB2 < 0) tensionB2 = 0;

    
    /*****************Informo tensiones*******************/
    Serial.print("Tensión B1 (V): ");
    Serial.println(tensionB1, 4);
    Serial.print("Tensión B2 (V): ");
    Serial.println(tensionB2, 4);
    
    /*****************Necesidad de balance****************/
    Serial.print("Balance: ");
    if(tensionB1 - tensionB2 > BALANCE_DELTA_V && tensionB1 > BALANCE_MIN_CELL_V && !balance2_request)
      if(balance1_request)
        Serial.println("Balance B1 in progress");
      else
        Serial.println("Balance B1 needed");
    else if(tensionB2 - tensionB1 > BALANCE_DELTA_V && tensionB2 > BALANCE_MIN_CELL_V && !balance1_request)
      if(balance2_request)
        Serial.println("Balance B2 in progress");
      else
        Serial.println("Balance B2 needed");
    else{
      if(balance1_request == true || balance2_request == true){
        balance1_request = false;
        balance2_request = false;
        Serial.println("DONE");
      }
      Serial.println("OK");
    }

    /****************Ejecuto protecciones******************/
    checkCellVoltageProtection(tensionB1, tensionB2);
    applyProteccion();

    medirVoltajes = false;
    Serial.println("|***************************************|");
  }
}

/********************************************************************************************************************************************************************************/
/****************************SETUP*******************************************/
void setupProtection() {
  pinMode(CHARGE_ENABLE_PIN, OUTPUT);
  pinMode(DISCHARGE_ENABLE_PIN, OUTPUT);
  digitalWrite(CHARGE_ENABLE_PIN, HIGH);    // HIGH = habilitado
  digitalWrite(DISCHARGE_ENABLE_PIN, HIGH); // HIGH = habilitado
}

void setupBalanceo(){
    // Balanceo
  pinMode(BAL1_PIN, OUTPUT);
  pinMode(BAL2_PIN, OUTPUT);
  digitalWrite(BAL1_PIN, LOW);
  digitalWrite(BAL2_PIN, LOW);
}


void setupTimer1() {
  cli();  // Desactiva interrupciones mientras se configura

  TCCR1A = 0;
  TCCR1B = 0;

  TCCR1B |= (1 << WGM12);               // Modo CTC
  TCCR1B |= (1 << CS11) | (1 << CS10);  // Prescaler 64

  OCR1A = 24999;                        // 100 ms (16MHz / 64 / 10Hz = 25000)

  TIMSK1 |= (1 << OCIE1A);              // Habilita interrupción por comparación

  sei();  // Habilita interrupciones
}
/******************************INTERRUPCIONES*****************************/
ISR(TIMER1_COMPA_vect) {
  medirCorriente = true;

  tickCounter++;
  if (tickCounter >= VOLTAGE_SAMPLE_TIME) {  // VOLTAGE_SAMPLE_TIME * 100ms
    medirVoltajes = true;
    tickCounter = 0;
  }
}

/***************************PROTECCIONES***********************************/
// Función de protección BMS
void checkCellVoltageProtection(float cell1_V, float cell2_V) {
  // Sobretensión celda 1
  if (cell1_V >= OVERVOLTAGE_LIMIT) {
    cell1OV = true;
  } 
  else if (cell1_V <= (OVERVOLTAGE_LIMIT - HYSTERESIS)) {
    cell1OV = false;
  }

  // Sobretensión celda 2
  if (cell2_V >= OVERVOLTAGE_LIMIT) {
    cell2OV = true;
  } 
  else if (cell2_V <= (OVERVOLTAGE_LIMIT - HYSTERESIS)) {
    cell2OV = false;
  }

  // Subtensión celda 1
  if (cell1_V <= UNDERVOLTAGE_LIMIT) {
    cell1UV = true;
  } 
  else if (cell1_V >= (UNDERVOLTAGE_LIMIT + HYSTERESIS)) {
    cell1UV = false;
  }

  // Subtensión celda 2
  if (cell2_V <= UNDERVOLTAGE_LIMIT) {
    cell2UV = true;
  } 
  else if (cell2_V >= (UNDERVOLTAGE_LIMIT + HYSTERESIS)) {
    cell2UV = false;
  }
}

void checkCellChargeCurrentProtection(float current){
  if(current < OVERCURRENT_IN_CHARGE && !chargeSC && !chargeOC){ //Si esta todo bien sale directamente y resetea contador
    overcurrentChargeCounter = 0;
    return;
  }else if(chargeSC || chargeOC){ //si no esta todo bien y salto algun flag
    if (overcurrentChargeCounter <= 0) overcurrentChargeCounter = HICCUPTIME; // seed
    if (overcurrentChargeCounter > 0)  overcurrentChargeCounter--;
    if(overcurrentChargeCounter <= 0){ //si se acaba reseteo flags
      chargeSC = false;
      chargeOC = false;
    }
  }else if(current >= SHORTCIRCUIT_IN_CHARGE){  //umbral de cortocircuito corta directamente
      chargeSC = true;
      overcurrentChargeCounter = HICCUPTIME;    
    }else{
      overcurrentChargeCounter++;
      if(overcurrentChargeCounter >= OCTIME){ //si llego a 20 corto la carga
        chargeOC = true;
        overcurrentChargeCounter = HICCUPTIME;
      }      
    }
  
  return;
}

void checkCellDischargeCurrentProtection(float current){
  if(current > OVERCURRENT_IN_DISCHARGE && !dischargeSC && !dischargeOC){ //Si esta todo bien sale directamente y resetea contador
    overcurrentDischargeCounter = 0;
    return;
  }else if(dischargeSC || dischargeOC){ //si no esta todo bien y salto algun flag 
    if (overcurrentDischargeCounter <= 0) overcurrentDischargeCounter = HICCUPTIME; // seed
    if (overcurrentDischargeCounter > 0)  overcurrentDischargeCounter--;
    if(overcurrentDischargeCounter <= 0){ //si se acaba reseteo flags
      dischargeSC = false;
      dischargeOC = false;
    }
  }else if(current <= SHORTCIRCUIT_IN_DISCHARGE){  //umbral de cortocircuito corta directamente
      dischargeSC = true;
      overcurrentDischargeCounter = HICCUPTIME;    
    }else{
      overcurrentDischargeCounter++;
      if(overcurrentDischargeCounter >= OCTIME){ //si llego a 20 corto la carga
        dischargeOC = true;
        overcurrentDischargeCounter = HICCUPTIME;
      }      
    }
  
  return;
}


void applyProteccion(){
  if(dischargeOC || dischargeSC || cell1UV || cell2UV)
    dischargeEnabled = false;
  else 
    dischargeEnabled = true;
  if(chargeOC || chargeSC || cell1OV || cell2OV)
    chargeEnabled = false;
  else 
    chargeEnabled = true;

  digitalWrite(CHARGE_ENABLE_PIN, chargeEnabled ? HIGH : LOW);
  digitalWrite(DISCHARGE_ENABLE_PIN, dischargeEnabled ? HIGH : LOW);
// --- Control de BALANCEO (aplicación SIEMPRE) ---
  
  //bool allowBal1 = !cell1OV && !cell1UV;   // regla básica: no balancear si OV/UV
  //bool allowBal2 = !cell2OV && !cell2UV;
  // (opcional) bloquear si hay OC/SC globales:
  // allowBal1 &= !(chargeSC || chargeOC || dischargeSC || dischargeOC);
  // allowBal2 &= !(chargeSC || chargeOC || dischargeSC || dischargeOC);

  bool bal1_on = balance1_request; // && allowBal1;
  bool bal2_on = balance2_request; // && allowBal2;

  digitalWrite(BAL1_PIN, bal1_on ? HIGH : LOW);
  digitalWrite(BAL2_PIN, bal2_on ? HIGH : LOW);

  // Debug 
  if(medirVoltajes){
    Serial.print("Flags - ChOv:"); Serial.print(chargeOC);
    Serial.print(" ChSh:"); Serial.print(chargeSC);
    Serial.print(" DcOv:"); Serial.print(dischargeOC);
    Serial.print(" DcSh:"); Serial.print(dischargeSC);
    Serial.print(" Cell1OV:"); Serial.print(cell1OV);
    Serial.print(" Cell2OV:"); Serial.print(cell2OV);
    Serial.print(" Cell1UV:"); Serial.print(cell1UV);
    Serial.print(" Cell2UV:"); Serial.println(cell2UV);

    Serial.print("Charge: "); Serial.println(chargeEnabled ? "ON" : "OFF");
    Serial.print("Discharge: "); Serial.println(dischargeEnabled ? "ON" : "OFF");
  }
/*
  // === NUEVO LOG SIEMPRE (balanceo) ===
  Serial.print(F("Balance req  B1:")); Serial.print(balance1_request);
  Serial.print(F(" B2:")); Serial.println(balance2_request);
  Serial.print(F("Balance appl B1:")); Serial.print(bal1_on ? "ON" : "OFF");
  Serial.print(F(" B2:")); Serial.println(bal2_on ? "ON" : "OFF");
*/
}
