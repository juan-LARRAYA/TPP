#include <Wire.h>
#include <Adafruit_INA219.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>
#include <EEPROM.h>


// ===== Estructuras =====
  struct BatteryCharge {
    double coulombCount1;   // En AVR (Uno), double = float (4 bytes)
    double coulombCount2;
  };

  struct PDUStatus {
    bool output33Selected;
    bool output33BSelected;
    bool output5Selected;
    bool output5BSelected;
    bool BatOutOn;
  };


// Objetos y constantes
  Adafruit_INA219 ina219BMS(0x40);
  Adafruit_INA219 ina219_33Out(0x41);
  Adafruit_INA219 ina219_5Out(0x44);
  Adafruit_INA219 ina219_BatOut(0x42);
  Adafruit_INA219 ina219_MPPTX(0x45);

// ****Prototipos*****//
  // ==== Setup / Inicialización ====
  void setupProtection();
  void setupBalanceo();
  void setupPDU();
  void setupTimer1();

  // ==== EEPROM ====
  void saveAllToEEPROM(int base = 0);
  void loadAllFromEEPROM(int base = 0);
  void saveOvervoltage(int base = 0);
  void loadOvervoltage(int base = 0);
  void saveBatteryCharge(int base = 0);
  void loadBatteryCharge(int base = 0);
  void savePDUStatus(int base = 0);
  void loadPDUStatus(int base = 0);

  // ==== Protecciones ====
  void checkCellVoltageProtection(float cell1_V, float cell2_V);
  void checkCellChargeCurrentProtection(float current);
  void checkCellDischargeCurrentProtection(float current);
  void applyProteccion();

  // ==== UART / Comandos ====
  void identificarSerial();



  const int VOLTAGE_ENABLE_PIN                = 7;
  const int ADC_DIVISOR_PIN                   = A0;  // Entrada del divisor

/********** Pines de control*********************/
  // ==== Habilitacion de carga y descarga
    const uint8_t CHARGE_ENABLE_PIN         = 8;
    const uint8_t DISCHARGE_ENABLE_PIN      = 9;

  // ==== Balanceo por comando
    const uint8_t BAL1_PIN                  = 10;   // Celda 1
    const uint8_t BAL2_PIN                  = 11;   // Celda 2

  // ==== Habilitaciones de Bucks

    const uint8_t ENABLE_3_OUT              = 3;
    const uint8_t ENABLE_3_BIS_OUT          = 4;

    const uint8_t ENABLE_5_OUT              = 5;
    const uint8_t ENABLE_5_BIS_OUT          = 6;
    
    const uint8_t ENABLE_BAT_OUT            = 12;

/************CONSTANTES*************************************/
  // Umbrales de protección (V)
    const float OVERVOLTAGE_LIMIT           = 4.20; // Overvoltage default
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
    const uint8_t MPPT_SAMPLE_TIME          = 10;

  // Configuración ADC
    const float REFERENCIA_INTERNA          = 1.1;    // 1.1V interna
    const float DIVISOR_FACTOR              = 4.328;   // Factor del divisor resistivo

  // Constantes del circuito
    const float RESBAL                      = 1000;   // 1Kohm de resistencia de balanceo

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
  
  // Control de Salidas FLAGS:
    bool Out33On =false;
    bool Out5On = false;
    PDUStatus pdu{};
    bool BatOutOn = false;

  //Control de flujo
  volatile bool medirCorriente  = false;
  volatile bool medirVoltajes   = false;
  volatile bool guardarStatus   = false;

  //Control de tiempo
  volatile unsigned int eepromCounter = 0;
  volatile unsigned int tickCounter = 0;
  unsigned long lastMeasureTime = 0;

  //Variables del sistema
    float corriente       = 0.0;
    float corriente33     = 0.0;
    float corriente5      = 0.0;
    float corrienteBat    = 0.0;
    float tensionINA      = 0.0;
    float tension33       = 0.0;
    float tension5        = 0.0;
    float tensionBat      = 0.0;
    float tensionADC      = 0.0;
    float tensionB1       = 0.0;
    float tensionB2       = 0.0;
    BatteryCharge battery{};
    float OvervoltageLimit = 0.0;




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

    loadAllFromEEPROM(); //cargo valores guardados en EEPROM

    lastMeasureTime = millis();
  }

/*********************************************************************/
/*****************MAIN LOOP¨****************************************/
  void loop() {
    sleep_mode();  // El micro duerme hasta que el Timer1 lo despierte

    // --- Se ejecuta al despertar ---

    while (Serial.available() > 0) {
      identificarSerial();
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
      if(digitalRead(BAL1_PIN)){
        battery.coulombCount1 += (corriente - tensionB1 / RESBAL) * deltaT_s;
      }else{
        battery.coulombCount1 += corriente * deltaT_s;      
      }
      if(digitalRead(BAL2_PIN)){
        battery.coulombCount2 += (corriente - tensionB2 / RESBAL) * deltaT_s;
      }else{
        battery.coulombCount2 += corriente * deltaT_s;      
      }
      medirCorriente = false;
    }

    if (medirVoltajes) {


      // Medir tensión con ina219BMS
      Serial.print("Corriente (mA): ");
      Serial.println(corriente);

      Serial.print("Carga acumulada B1 (mAh): ");
      Serial.println(battery.coulombCount1 / 3600.0, 5);
      Serial.print("Carga acumulada B2 (mAh): ");
      Serial.println(battery.coulombCount2 / 3600.0, 5);

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

    if (guardarStatus){
      saveBatteryCharge();
      guardarStatus = false;
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

  void setupPDU(){
      // Balanceo
    pinMode(ENABLE_3_OUT, OUTPUT);
    pinMode(ENABLE_3_BIS_OUT, OUTPUT);
    pinMode(ENABLE_5_OUT, OUTPUT);
    pinMode(ENABLE_5_BIS_OUT, OUTPUT);
    pinMode(ENABLE_BAT_OUT, OUTPUT);
    
    digitalWrite(ENABLE_3_OUT, LOW);
    digitalWrite(ENABLE_3_BIS_OUT, LOW);
    digitalWrite(ENABLE_5_OUT, LOW);
    digitalWrite(ENABLE_5_BIS_OUT, LOW);
    digitalWrite(ENABLE_BAT_OUT, LOW);
    
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
    eepromCounter++;
    tickCounter++;
    if (tickCounter >= VOLTAGE_SAMPLE_TIME) {  // VOLTAGE_SAMPLE_TIME * 100ms
      medirVoltajes = true;
      tickCounter = 0;
    }
    if (eepromCounter >= 18000){
      eepromCounter = 0;
      guardarStatus = true;
    }
  }


/*******************EEPROM**************************************************************************************************/
  // =====================================================
  // Helpers internos: calculan offsets LOCALMENTE
  // Orden fijo: OvervoltageLimit -> BatteryCharge -> PDUStatus
  // =====================================================
  static inline void calcOffsets(int base, int &offOV, int &offBat, int &offPDU) {
    int off = base;
    offOV  = off;                              off += sizeof(OvervoltageLimit);
    offBat = off;                              off += sizeof(BatteryCharge);
    offPDU = off;
  }

  // ===== Guardar / Cargar todo =====
  void saveAllToEEPROM(int base = 0) {
    int offOV, offBat, offPDU;
    calcOffsets(base, offOV, offBat, offPDU);

    EEPROM.put(offOV,  OvervoltageLimit);
    EEPROM.put(offBat, battery);
    EEPROM.put(offPDU, pdu);
  }

  void loadAllFromEEPROM(int base = 0) {
    int offOV, offBat, offPDU;
    calcOffsets(base, offOV, offBat, offPDU);

    EEPROM.get(offOV,  OvervoltageLimit);
    EEPROM.get(offBat, battery);
    EEPROM.get(offPDU, pdu);
  }

  // ===== Guardar / Cargar cada bloque por separado =====
  void saveOvervoltage(int base = 0) {
    int offOV, offBat, offPDU;
    calcOffsets(base, offOV, offBat, offPDU);
    EEPROM.put(offOV, OvervoltageLimit);
  }

  void loadOvervoltage(int base = 0) {
    int offOV, offBat, offPDU;
    calcOffsets(base, offOV, offBat, offPDU);
    EEPROM.get(offOV, OvervoltageLimit);
  }

  void saveBatteryCharge(int base = 0) {
    int offOV, offBat, offPDU;
    calcOffsets(base, offOV, offBat, offPDU);
    EEPROM.put(offBat, battery);
  }

  void loadBatteryCharge(int base = 0) {
    int offOV, offBat, offPDU;
    calcOffsets(base, offOV, offBat, offPDU);
    EEPROM.get(offBat, battery);
  }

  void savePDUStatus(int base = 0) {
    int offOV, offBat, offPDU;
    calcOffsets(base, offOV, offBat, offPDU);
    EEPROM.put(offPDU, pdu);
  }

  void loadPDUStatus(int base = 0) {
    int offOV, offBat, offPDU;
    calcOffsets(base, offOV, offBat, offPDU);
    EEPROM.get(offPDU, pdu);
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

  void applyPDU() {
    // --- Salida 3.3V ---
    if (!Out33On) {
      digitalWrite(ENABLE_3_OUT, LOW);
      digitalWrite(ENABLE_3_BIS_OUT, LOW);
    } else {
      if (pdu.output33Selected && !pdu.output33BSelected) {
        digitalWrite(ENABLE_3_OUT, HIGH);
        digitalWrite(ENABLE_3_BIS_OUT, LOW);
      } else if (!pdu.output33Selected && pdu.output33BSelected) {
        digitalWrite(ENABLE_3_OUT, LOW);
        digitalWrite(ENABLE_3_BIS_OUT, HIGH);
      } else {
        // caso inválido: ambos true o ambos false
        digitalWrite(ENABLE_3_OUT, LOW);
        digitalWrite(ENABLE_3_BIS_OUT, LOW);
      }
    }

    // --- Salida 5V ---
    if (!Out5On) {
      digitalWrite(ENABLE_5_OUT, LOW);
      digitalWrite(ENABLE_5_BIS_OUT, LOW);
    } else {
      if (pdu.output5Selected && !pdu.output5BSelected) {
        digitalWrite(ENABLE_5_OUT, HIGH);
        digitalWrite(ENABLE_5_BIS_OUT, LOW);
      } else if (!pdu.output5Selected && pdu.output5BSelected) {
        digitalWrite(ENABLE_5_OUT, LOW);
        digitalWrite(ENABLE_5_BIS_OUT, HIGH);
      } else {
        // caso inválido
        digitalWrite(ENABLE_5_OUT, LOW);
        digitalWrite(ENABLE_5_BIS_OUT, LOW);
      }
    }

    // --- Salida Bat ---
    if (BatOutOn) {
      digitalWrite(ENABLE_BAT_OUT, HIGH);
    } else {
      digitalWrite(ENABLE_BAT_OUT, LOW);
    }
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

    applyPDU();



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
/***************************UART***********************************/
  void identificarSerial(){
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    int len = cmd.length();
    if (len <= 2)
      return;

    switch(cmd[0]){
      case 'M': //Todo lo referido a MPPT
        if (len > 3){
          Serial.println("Invalid Command");
          return;
        }
        switch(cmd[1]){ //identifico siguiente byte
          case 'R': // registro de estado
            if (len > 2){
              Serial.println("Invalid Command");
              return;
            }else{
              Serial.println("devolver registro mppt");
            }
          break;

          case 'X': // referido al par de paneles del eje X
            switch(cmd[2]){
              case 'N': // encender mppt x
              Serial.println("MPPT X: ON");
              break;
              case 'F': // apagar mppt x
              Serial.println("MPPT X: OFF");
              break;
              case 'V': // tension mppt x
              Serial.println("VX: ");
              break;
              case 'C': // corriente mppt x
              Serial.println("VX: ");
              break;
              default: // cualquier cotra cosa es invalido
              Serial.println("Invalid Command");
              return;
              break;
            }
            break;
            case 'Y': // referido al par de paneles del eje Y
            switch(cmd[2]){
              case 'N': // encender mppt y
              Serial.println("MPPT Y: ON");
              break;
              case 'F': // apagar mppt y
              Serial.println("MPPT Y: OFF");
              break;
              case 'V': // tension mppt y
              Serial.println("VY: ");
              break;
              case 'C': // corriente mppt y
              Serial.println("VY: ");
              break;
              default: // cualquier cotra cosa es invalido
              Serial.println("Invalid Command");
              return;
              break;
            }
            break;
            case 'Z': // referido al par de paneles del eje Z
            switch(cmd[2]){
              case 'N': // encender mppt z
              Serial.println("MPPT Z: ON");
              break;
              case 'F': // apagar mppt z
              Serial.println("MPPT Z: OFF");
              break;
              case 'V': // tension mppt z
              Serial.println("VZ: ");
              break;
              case 'C': // corriente mppt z
              Serial.println("VZ: ");
              break;
              default: // cualquier cotra cosa es invalido
              Serial.println("Invalid Command");
              return;
              break;
            }
            break;
          default:
          Serial.println("Invalid Command");
          break;
        }
        break;
      case 'P': //Todo lo referido a PDU
        if (len > 3){
          Serial.println("Invalid Command");
          return;
        }
        switch(cmd[1]){ //identifico siguiente byte

          case '3': // referido a la salida de 3.3V
            switch(cmd[2]){
              case 'N': // encender mppt x
                Serial.println("3.3V Output: ON");
                Out33On = true;
                if((pdu.output33Selected == false && pdu.output33BSelected == false) || (pdu.output33Selected == true && pdu.output33BSelected == true)){
                  pdu.output33Selected == true;
                  pdu.output33BSelected == false;
                  savePDUStatus();
                }
              break;
              case 'F': // apagar mppt x
                Serial.println("3.3V Outout: OFF");
                Out33On = false;
              break;
              case 'S': // tension mppt x
                Serial.println("Alternar backup");
                if(pdu.output33Selected == true && pdu.output33BSelected == false){
                  pdu.output33Selected == false;
                  pdu.output33BSelected == true;
                  savePDUStatus();
                }else if(pdu.output33Selected == false && pdu.output33BSelected == true){
                  pdu.output33Selected == true;
                  pdu.output33BSelected == false;
                  savePDUStatus();
                }
              break;
              case 'V': // tension mppt x
                Serial.print("V 3.3: ");
                tension33 = ina219_33Out.getBusVoltage_V();
                Serial.println(tension33);
              break;
              case 'C': // corriente mppt x
                Serial.print("C 3.3: ");
                corriente33 = ina219_33Out.getCurrent_mA();
                Serial.println(corriente33);
              break;
              default: // cualquier cotra cosa es invalido
                Serial.println("Invalid Command");
              return;
              break;
            }
            break;
            case '5': // referido a la salida de 5V
            switch(cmd[2]){
              case 'N': // encender mppt x
                Serial.println("5V Output: ON");
                Out5On = true;
                if((pdu.output5Selected == false && pdu.output5BSelected == false) || (pdu.output5Selected == true && pdu.output5BSelected == true)){
                  pdu.output5Selected == true;
                  pdu.output5BSelected == false;
                  savePDUStatus();
                }
              break;
              case 'F': // apagar mppt x
                Serial.println("5V Outout: OFF");
                Out5On = false;
              break;
              case 'S': // tension mppt x
                Serial.println("Alternar backup");
                if(pdu.output5Selected == true && pdu.output5BSelected == false){
                  pdu.output5Selected == false;
                  pdu.output5BSelected == true;
                  savePDUStatus();
                }else if(pdu.output5Selected == false && pdu.output5BSelected == true){
                  pdu.output5Selected == true;
                  pdu.output5BSelected == false;
                  savePDUStatus();
                }
              break;
              case 'V': // tension mppt x
                Serial.println("V 5: ");
                tension5 = ina219_5Out.getBusVoltage_V();
                Serial.println(tension5);
              break;
              case 'C': // corriente mppt x
                Serial.println("C 5: ");
                corriente5 = ina219_5Out.getCurrent_mA();
                Serial.println(corriente5);
              break;
              default: // cualquier cotra cosa es invalido
                Serial.println("Invalid Command");
              return;
              break;
            }
            break;
            case 'B': // referido a la salida de Bat+
            switch(cmd[2]){
              case 'N': // encender mppt x
                Serial.println("Bat Output: ON");
                BatOutOn = true;
              break;
              case 'F': // apagar mppt x
                Serial.println("Bat Outout: OFF");
                BatOutOn = false;
              break;
              case 'V': // tension mppt x
                Serial.println("V Bat: ");
                tensionBat = ina219_BatOut.getBusVoltage_V();
                Serial.println(tensionBat);
              break;
              case 'C': // corriente mppt x
                Serial.println("C Bat: ");
                corrienteBat = ina219_BatOut.getCurrent_mA();
                Serial.println(corrienteBat);
              break;
              default: // cualquier cotra cosa es invalido
                Serial.println("Invalid Command");
              return;
              break;
            }
            break;
          default:
          Serial.println("Invalid Command");
          break;
        }
        break;
      case 'B': //Todo lo referido a BMS
        if (len > 3){
          Serial.println("Invalid Command");
          return;
        }
        switch(cmd[1]){ //identifico siguiente byte
          case 'S': // registro de estado
            if (len > 2){
              Serial.println("Invalid Command");
              return;
            }else{
              Serial.println("devolver registros BMS");
            }
            break;
          case 'C': // Corriente de las baterias
            if (len > 2){
              Serial.println("Invalid Command");
              return;
            }else{
              Serial.print("Corriente Baterias:");
              Serial.println(corriente);
            }
            break;
          case 'T': // Temperatura de las baterias
            if (len > 2){
              Serial.println("Invalid Command");
              return;
            }else{
              Serial.println("Temperatura Baterias:");
            }
            break;
          /*
          case '%': // Porcentaje de carga de celdas
            if (len > 2){
              Serial.println("Invalid Command");
              return;
            }else{
              Serial.println("Porcentaje de carga de las baterias:");
            }
            break;
          */
          case '1': // referido a la bateria 1
              switch(cmd[2]){
                case 'N': // encender Balance 1
                balance1_request = true;
                Serial.println("Bal Bat 1: ON");
                break;
                case 'F': // apagar Balance 1
                balance1_request = false;
                Serial.println("Bal Bat 1: OFF");
                break;
                case 'V': // tension Bateria 1
                Serial.print("Tensión B1 (V): ");
                Serial.println(tensionB1, 4);
                break;
                case 'Q': // Carga Bateria 1
                Serial.print("Carga B1 (mAh): ");
                Serial.println(battery.coulombCount1 / 3600.0, 5);
                break;
                default: // cualquier cotra cosa es invalido
                Serial.println("Invalid Command");
                return;
                break;
              }
              break;
          case '2': // referido a la bateria 2
            switch(cmd[2]){
              case 'N': // encender Balance 2
              balance2_request = true;
              Serial.println("Bal Bat 2: ON");
              break;
              case 'F': // apagar Balance 2
              balance2_request = false;
              Serial.println("Bal Bat 2: OFF");
              break;
              case 'V': // tension Bateria 2
              Serial.print("Tensión B2 (V): ");
              Serial.println(tensionB2, 4);
              break;
              case 'Q': // Carga Bateria 2
              Serial.print("Carga B2 (mAh): ");
              Serial.println(battery.coulombCount2 / 3600.0, 5);
              break;
              default: // cualquier cotra cosa es invalido
              Serial.println("Invalid Command");
              return;
              break;
            }
            break;
          case 'V': // Umbral de sobretension
            switch(cmd[2]){
              case '0': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 3.90;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case '1': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 3.95;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case '2': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 4.0;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case '3': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 4.05;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case '4': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 4.1;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case '5': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 4.15;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case '6': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 4.2;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case '7': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 4.25;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case '8': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 4.3;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              /*
              case '9': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 3.95;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case 'A': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 3.95;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case 'B': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 3.95;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case 'C': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 3.95;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case 'D': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 3.95;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case 'E': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 3.95;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              case 'F': 
              Serial.print("OV Voltage: ");
              OvervoltageLimit = 3.95;
              Serial.println(OvervoltageLimit, 4);
              saveOvervoltage();
              break;
              */
              default: 
              Serial.println("Invalid Command");
              return;
              break;
            }
            break;
          default:
          Serial.println("Invalid Command");
          break;
        }
        break;
      default:
        Serial.println("Invalid Command");
        break;
    }
  }
