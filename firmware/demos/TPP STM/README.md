# TPP - Sistema de Gestión de Energía para Satélites Cubesat

## 📋 Descripción del Proyecto

Este repositorio contiene el desarrollo completo de un **Sistema de Gestión de Energía (EPS - Electrical Power System)** modular para satélites Cubesat, desarrollado como parte de una tesis de ingeniería electrónica. El proyecto incluye múltiples subsistemas integrados: **BMS (Battery Management System)**, **MPPT (Maximum Power Point Tracking)**, **PDU (Power Distribution Unit)** y herramientas de monitoreo y análisis de datos.

## 🎯 Objetivos del Proyecto

- **Desarrollo de un BMS completo** con protección de baterías Li-ion
- **Implementación de algoritmo MPPT** para optimización de paneles solares
- **Sistema de distribución de energía** con múltiples fuentes
- **Monitoreo en tiempo real** de parámetros eléctricos
- **Análisis de datos** para caracterización de baterías
- **Comunicación I2C** entre subsistemas

## 🏗️ Arquitectura del Sistema

### Subsistemas Principales

1. **BMS (Battery Management System)**
   - Protección por sobretensión/subtensión
   - Protección por sobrecorriente/cortocircuito
   - Balanceo de celdas
   - Coulomb counting para medición de capacidad

2. **MPPT (Maximum Power Point Tracking)**
   - Algoritmo Perturbar y Observar
   - Optimización de potencia de paneles solares
   - Control PWM de alta frecuencia

3. **PDU (Power Distribution Unit)**
   - Distribución de energía a múltiples cargas
   - Medición de eficiencia de fuentes
   - Control de encendido/apagado de subsistemas

4. **Sistema de Monitoreo**
   - Interfaz gráfica en Python
   - Adquisición de datos en tiempo real
   - Análisis y visualización de datos

## 📁 Estructura Detallada del Proyecto

### 📁 **arduino VARIOS/** - Desarrollo Arduino y Demostraciones
Esta carpeta contiene todo el código de desarrollo inicial y las demostraciones del proyecto:

#### **📁 demos/** - Demostraciones y Pruebas
- **📁 DuinoSTM32/** - Comunicación entre Arduino y STM32 con BQ
  - `BQComunication.ino` - Protocolo de comunicación con BQ29330
  - `filmwareSTM32.ino` - Firmware para STM32
  - Imágenes de configuración y comandos BQ
- **📁 filmwareSTM32/** - Duplicado del firmware STM32
- **📁 pruebasMppt/** - Pruebas del algoritmo MPPT
  - **📁 arduino/** - Implementación Arduino del MPPT
  - **📁 bluePIlPrueba/** - Pruebas en STM32 Blue Pill
  - **📁 esp32/** - Implementaciones ESP32 con múltiples PWM
- **📁 stm32_binaries/** - Binarios precompilados para diferentes STM32
- `juan.c` - Código C de desarrollo inicial
- `virtualEprom.c` - Simulación de memoria EEPROM

#### **📁 piserial/** - Sistema de Monitoreo y Análisis
- **📁 ui/** - Interfaces gráficas Python
  - `piserial_gui_simple.py` - GUI simplificada (recomendada)
  - `piserial_gui.py` - GUI avanzada con gráficos
  - Scripts de ejecución y dependencias
- **📁 graficos/** - Datos reales de pruebas de baterías
  - **📁 carga 0,5 con errores/** - Datos de carga a 0.5C con análisis de errores
  - **📁 descarga 0,2/** - Datos de descarga a 0.2C
  - **📁 descarga 0,2 megalite/** - Pruebas con batería Megalite
- `piserial.py` - Monitor serial básico
- `serial2csv.py` - Conversor de datos serial a CSV
- Archivos CSV con datos reales de sensores

#### **📁 readIna219/** - Lectura de Sensores
- `readIna219.ino` - Código para leer sensores INA219 con coulomb counting

#### **📁 TPP_24_8_25 4/** - Versiones de Agosto 2025
- **📁 SEND_PWD_ARDUINO_TO_STM32/** - Comunicación Arduino→STM32
- **📁 TPP_BMS_NICO/** - BMS desarrollado por Nicolás

#### **Archivos de Desarrollo**
- `adc_ino/` - Pruebas de ADC
- `com_i2c_stm32/` - Comunicación I2C con STM32
- `sensor_data.csv` - Datos de sensores

### 📁 **STM/** - Proyectos STM32CubeIDE
Desarrollo profesional con STM32CubeIDE:

#### **📁 PWD_ARDUINO_SLAVE/** - Proyecto Principal STM32
- **📁 Core/** - Código fuente principal
  - **📁 Inc/** - Archivos de cabecera (.h)
  - **📁 Src/** - Archivos fuente (.c)
  - **📁 Startup/** - Código de inicialización
- **📁 Debug/** - Archivos compilados y de depuración
- `PWD_ARDUINO_SLAVE.ioc` - Configuración STM32CubeMX

#### **📁 otros STM32/** - Variantes y Pruebas
- **📁 BLUEPILL ORIGINAL/** - STM32F103 original
- **📁 BLUEPILL_CHINA/** - STM32F103 chino (128k)
- **📁 BQ29330_BLUEPILL_ORIGINAL/** - Integración con BQ29330
- **📁 Prueba ADC/** - Pruebas de conversión ADC
- **📁 st405 64patas/** - STM32F405 de 64 pines
- Otros proyectos de prueba y desarrollo

#### **📁 workspace_1.16.1/** - Workspace STM32CubeIDE
- **📁 STM32F405_EPS/** - Sistema EPS completo en STM32F405

### 📁 **final/** - Versiones Finales del Proyecto
Código final consolidado y probado:

#### **📁 TPP_BMS_NICO/** - BMS Final
- `TPP_BMS_NICO.ino` - Sistema BMS completo desarrollado por Nicolás
- Incluye protecciones, balanceo y coulomb counting

#### **📁 PWD_ARDUINO_SLAVE/** - STM32 Final
- Código STM32 final para comunicación I2C
- Proyecto completo con todas las funcionalidades

#### **📁 esp32/** - Implementación ESP32
- `esp32.ino` - Versión ESP32 del BMS con mejoras de rendimiento

#### **📄 Pruebas enrique.md** - Notas de Pruebas
- Lista de pruebas pendientes con Enrique
- Validación de protecciones y comunicaciones

### 📄 **TO DO TPP.md** - Lista de Tareas y Especificaciones
- Lista completa de tareas del proyecto
- Especificaciones técnicas detalladas
- Requerimientos de pruebas y validación

## 🔧 Tecnologías Utilizadas

### Hardware
- **Microcontroladores**: STM32F103, STM32F405, ESP32, Arduino
- **Sensores**: INA219 (medición de corriente/tensión)
- **BMS**: BQ76905, BQ29330
- **Comunicación**: I2C, UART, PWM

### Software
- **Desarrollo**: STM32CubeIDE, Arduino IDE
- **Monitoreo**: Python (tkinter, matplotlib, pyserial)
- **Análisis**: CSV, Excel, gráficos de datos

## 🚀 Instalación y Configuración

### 1. Clonar el Repositorio
```bash
git clone https://github.com/juan-LARRAYA/TPP.git
cd TPP
```

### 2. Configuración del Entorno STM32
1. Instalar **STM32CubeIDE**
2. Importar proyectos desde `STM/` o `final/PWD_ARDUINO_SLAVE/`
3. Configurar el debugger (ST-Link)
4. Compilar y cargar el firmware

### 3. Sistema de Monitoreo Python
```bash
cd "arduino VARIOS/piserial/ui"

# Crear entorno virtual
python3 -m venv venv
source venv/bin/activate

# Instalar dependencias
pip install -r requirements.txt

# Ejecutar interfaz gráfica
./run_simple_gui.sh
```

## 📊 Funcionalidades Principales

### BMS (Battery Management System)
- ✅ **Protección por voltaje**: UV/OV con histéresis
- ✅ **Protección por corriente**: OC/SC en carga y descarga
- ✅ **Balanceo de celdas**: Control automático y manual
- ✅ **Coulomb counting**: Medición precisa de capacidad
- ✅ **Comunicación I2C**: Integración con otros subsistemas

### MPPT (Maximum Power Point Tracking)
- ✅ **Algoritmo P&O**: Perturbar y Observar
- ✅ **Control PWM**: Alta frecuencia (62 kHz)
- ✅ **Medición de potencia**: Entrada y salida
- ✅ **Optimización automática**: Seguimiento del punto máximo

### Sistema de Monitoreo
- ✅ **Interfaz gráfica**: Tkinter con tabla de datos
- ✅ **Adquisición en tiempo real**: Puerto serie
- ✅ **Guardado automático**: Formato CSV
- ✅ **Análisis de datos**: Gráficos y estadísticas
- ✅ **Múltiples versiones**: GUI simple y avanzada

## 📈 Datos y Resultados

El proyecto incluye datos reales de pruebas de baterías:

### Pruebas de Carga
- **Carga a 0.2C**: Datos de tensión y corriente
- **Carga a 0.5C**: Curvas de carga con errores documentados
- **Análisis de eficiencia**: Comparación de fuentes

### Pruebas de Descarga
- **Descarga a 0.2C**: Caracterización de capacidad
- **Descarga a 0.5C**: Validación de protecciones
- **Corte por UV**: Verificación de límites de seguridad

### Archivos de Datos
- `sensor_carga_samsung_20250707.csv`: Datos de carga Samsung
- `graficos/carga 0,5 con errores/`: Análisis de errores en carga
- `graficos/descarga 0,2/`: Datos de descarga a corriente constante

## 🎮 Uso del Sistema

### Comandos BMS (Puerto Serie)
- `a` / `A`: Activar/desactivar balanceo celda 1
- `b` / `B`: Activar/desactivar balanceo celda 2
- `x`: Desactivar ambos balanceos
- `s`: Solicitar estado del sistema

### Interfaz de Monitoreo
1. **Conectar**: Seleccionar puerto serie
2. **Monitorear**: Ver datos en tiempo real
3. **Analizar**: Revisar gráficos y estadísticas
4. **Exportar**: Guardar datos en CSV

## 🔬 Especificaciones Técnicas

### BMS
- **Voltaje de trabajo**: 3.3V - 4.2V por celda
- **Corriente máxima**: ±1200mA (OC), ±1400mA (SC)
- **Precisión**: ±50mV histéresis
- **Balanceo**: Automático con diferencia >80mV

### MPPT
- **Frecuencia PWM**: 62 kHz
- **Resolución**: 8 bits (0-255)
- **Algoritmo**: Perturbar y Observar
- **Incremento**: 1 unidad por iteración

### Sistema de Monitoreo
- **Frecuencia de muestreo**: 10 Hz (corriente), 0.2 Hz (voltaje)
- **Precisión ADC**: 12 bits (ESP32), 10 bits (Arduino)
- **Comunicación**: UART 115200 baud

## 🐛 Problemas Conocidos y Soluciones

### BMS
- **BQ29330 lento en regular**: Reiniciar fuente de alimentación
- **I2C no funciona**: Descargar capacitores con tester
- **Falta de fuel gauge**: Implementado con timer y coulomb counting

### Sistema de Monitoreo
- **Error Tcl/Tk en macOS**: Usar versión GUI simplificada
- **Puerto no encontrado**: Verificar conexión y cambiar puerto
- **Datos corruptos**: Verificar baudrate y conexión

## 📋 Estado del Proyecto

### ✅ Completado
- [x] Implementación básica del BMS
- [x] Algoritmo MPPT funcional
- [x] Sistema de monitoreo Python
- [x] Comunicación I2C entre placas
- [x] Pruebas de carga y descarga
- [x] Análisis de datos y gráficos

### 🔄 En Progreso
- [ ] Configuración completa del BMS
- [ ] Optimización de eficiencia de fuentes
- [ ] Integración completa de subsistemas
- [ ] Documentación de comandos I2C


## 👥 Equipo de Desarrollo

**Tesis de Ingeniería Electrónica - Proyecto Conjunto:**

Este proyecto fue desarrollado de manera colaborativa entre tres estudiantes de ingeniería electrónica, cada uno contribuyendo con sus especialidades y conocimientos:


- **Larraya, Juan Cruz - jlarraya@fi.uba.ar** 
- **Stirparo, Nicolás - nstirparo@fi.uba.ar**
- **Paulino Alaya, Diego - dpaulino@fi.uba.ar** 

## 📚 Referencias y Documentación

- **STM32CubeIDE**: [Documentación oficial](https://www.st.com/en/development-tools/stm32cubeide.html)
- **INA219**: [Hoja de datos Adafruit](https://www.adafruit.com/product/904)
- **BQ76905**: [Hoja de datos TI](https://www.ti.com/product/BQ76905)
- **Cubesat Standards**: [NASA Cubesat Design Specification](https://www.nasa.gov/cubesat)

## 📄 Licencia

Este proyecto es desarrollado como parte de una tesis de ingeniería electrónica. Todos los derechos reservados.

## 🤝 Contribuciones

Para contribuir al proyecto:
1. Fork el repositorio
2. Crear una rama para tu feature
3. Commit tus cambios
4. Push a la rama
5. Crear un Pull Request

---

**Nota**: Este proyecto está en desarrollo activo. Para la versión más reciente, consultar la rama `main` del repositorio.
