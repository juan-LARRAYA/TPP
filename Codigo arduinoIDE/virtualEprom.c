#include "stm32f1xx_hal.h"

// Definiciones
#define EEPROM_START_ADDRESS 0x08020000 // Dirección de inicio de la EEPROM emulada
#define EEPROM_SIZE 16384                // 16 KB de espacio reservado

// Función para escribir un byte en la EEPROM emulada
void EEPROM_Write(uint32_t address, uint8_t data) {
    if (address >= EEPROM_START_ADDRESS && address < (EEPROM_START_ADDRESS + EEPROM_SIZE)) {
        HAL_FLASH_Unlock();                     // Desbloquear Flash
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, address, data); // Escribir el byte (TYPEPROGRAM_BYTE, TYPEPROGRAM_HALFWORD, TYPEPROGRAM_WORD)
        HAL_FLASH_Lock();                       // Bloquear Flash
    }
}

// Función para leer un byte de la EEPROM emulada
uint8_t EEPROM_Read(uint32_t address) {
    if (address >= EEPROM_START_ADDRESS && address < (EEPROM_START_ADDRESS + EEPROM_SIZE)) {
        return *(volatile uint8_t*)address;    // Leer directamente desde la dirección
    }
    return 0xFF; // Devuelve un valor de error si está fuera de rango
}

// Función principal
int main(void) {
    HAL_Init(); // Inicializar HAL

    // Escribir datos
    EEPROM_Write(EEPROM_START_ADDRESS, 0xAA); // Escribir 0xAA
    EEPROM_Write(EEPROM_START_ADDRESS + 1, 0x55); // Escribir 0x55

    // Leer datos
    uint8_t value1 = EEPROM_Read(EEPROM_START_ADDRESS);       // Leer el primer byte
    uint8_t value2 = EEPROM_Read(EEPROM_START_ADDRESS + 1);   // Leer el segundo byte

    while (1) {
        // Loop infinito
    }
}
