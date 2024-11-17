#include "stm32f1xx_hal.h"  // Asegúrate de incluir la biblioteca HAL adecuada

#define BQ76905_ADDR (0x08 << 1) // Dirección del BQ76905 desplazada a la izquierda para I2C
I2C_HandleTypeDef hi2c1;

// Estructura para almacenar los datos de los registros
typedef struct {
    uint16_t cell1_voltage;        // Voltaje de la celda 1 (mV)
    uint16_t cell2_voltage;        // Voltaje de la celda 2 (mV)
    uint8_t alert_status_A;        // Registro de alertas A
    uint8_t fault_status_A;        // Registro de fallas A
    uint8_t alert_status_B;        // Registro de alertas B
    uint8_t fault_status_B;        // Registro de fallas B
    uint16_t battery_status;       // Registro de estado de la batería
} BatteryData;

void MX_I2C1_Init(void) {
    hi2c1.Instance = I2C1;
    hi2c1.Init.ClockSpeed = 100000;
    hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK) {
        // Error handling
    }
}


// Función para leer un registro específico del BQ76905
HAL_StatusTypeDef BQ76905_ReadRegister(uint8_t reg, uint8_t* data, uint16_t len) {
    HAL_StatusTypeDef status;

    // Enviar el registro desde el cual leeremos
    status = HAL_I2C_Master_Transmit(&hi2c1, BQ76905_ADDR, &reg, 1, HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return status; // Retorna el error si no se pudo enviar el registro
    }

    // Leer los datos del registro
    return HAL_I2C_Master_Receive(&hi2c1, BQ76905_ADDR, data, len, HAL_MAX_DELAY);
}

// Función para leer los datos de la batería y almacenarlos en la estructura BatteryData
BatteryData ReadBatteryData(void) {
    BatteryData data; // Estructura para almacenar los datos
    uint8_t buffer[2]; // Buffer para leer datos de 16 bits

    // Leer el voltaje de la celda 1
    if (BQ76905_ReadRegister(0x14, buffer, 2) == HAL_OK) {
        data.cell1_voltage = (buffer[0] << 8) | buffer[1];
    } else {
        data.cell1_voltage = 0; // Manejo básico de error
    }

    // Leer el voltaje de la celda 2
    if (BQ76905_ReadRegister(0x16, buffer, 2) == HAL_OK) {
        data.cell2_voltage = (buffer[0] << 8) | buffer[1];
    } else {
        data.cell2_voltage = 0; // Manejo básico de error
    }

    // Leer el registro de alertas A
    if (BQ76905_ReadRegister(0x02, &data.alert_status_A, 1) != HAL_OK) {
        data.alert_status_A = 0; // Manejo básico de error
    }

    // Leer el registro de fallas A
    if (BQ76905_ReadRegister(0x03, &data.fault_status_A, 1) != HAL_OK) {
        data.fault_status_A = 0; // Manejo básico de error
    }

    // Leer el registro de alertas B
    if (BQ76905_ReadRegister(0x04, &data.alert_status_B, 1) != HAL_OK) {
        data.alert_status_B = 0; // Manejo básico de error
    }

    // Leer el registro de fallas B
    if (BQ76905_ReadRegister(0x05, &data.fault_status_B, 1) != HAL_OK) {
        data.fault_status_B = 0; // Manejo básico de error
    }

    // Leer el registro de estado de la batería
    if (BQ76905_ReadRegister(0x12, buffer, 2) == HAL_OK) {
        data.battery_status = (buffer[0] << 8) | buffer[1];
    } else {
        data.battery_status = 0; // Manejo básico de error
    }

    return data;
}

// Función principal
int main(void) {
    HAL_Init();             // Inicializa la HAL
    SystemClock_Config();   // Configura el reloj del sistema
    MX_I2C1_Init();         // Inicializa el periférico I2C

    BatteryData battery_data; // Estructura para almacenar los datos de la batería

    while (1) {
        battery_data = ReadBatteryData();

        // Aquí puedes procesar los datos, enviarlos a una interfaz, etc.
        HAL_Delay(1000); // Espera 1 segundo antes de leer nuevamente
    }
}

