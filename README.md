Aquí tienes un ejemplo de un archivo README para tu repositorio en GitHub. Puedes adaptarlo según las necesidades específicas de tu proyecto:

---

# Proyecto de Firmware para STM32 - Tesis de Ingeniería Electrónica

## Descripción del Proyecto  
Este repositorio contiene el código fuente para usar y probar una EPS modular para satelites de cubesats, desarrollado como parte de nuestra tesis de ingeniería electrónica. 

El desarrollo se realiza utilizando **STM32CubeIDE**, una herramienta integrada que facilita la configuración y programación de los microcontroladores STM32.

---

## Características Principales  
- **Plataformas Soportadas**:  
  - STM32F405 (versión principal del proyecto).  
  - STM32F103 (pruebas y prototipos).  
  - STM32 "chino" (para experimentación y pruebas adicionales).  
- **Demos Iniciales**:  
  - Implementadas en **Arduino** como referencia para las funcionalidades iniciales del sistema.  
- **Entorno de Desarrollo**:  
  - Todo el código está estructurado y desarrollado en **STM32CubeIDE**, con soporte para configuraciones específicas de cada microcontrolador.



## Instalación y Configuración  
1. **Clona el repositorio**:  
   ```bash
   git clone https://github.com/juan-LARRAYA/TPP.git
   ```
2. **Importa el proyecto en STM32CubeIDE**:  
   - Abre STM32CubeIDE.  
   - Selecciona `File > Open Projects from File System`.  
   - Navega a la carpeta correspondiente (F405, F103, etc.) y selecciona el proyecto.  

3. **Compila y sube el código**:  
   - Conecta el microcontrolador al equipo mediante un programador/debugger compatible (ej. ST-Link, TTL-USB).  
   - Configura el entorno de depuración y carga en `Run > Debug`.  

---

## Uso y Ejecución  
1. Selecciona la versión del firmware adecuada para tu microcontrolador (F405, F103, o STM32 "chino").  
2. Compila y sube el firmware utilizando **STM32CubeIDE**.  
3. Verifica el funcionamiento con las pruebas establecidas en cada módulo del proyecto.  

---

## Créditos  
Equipo de tesis de ingeniería electrónica:  
- **Juan Larraya**  
- **Diego Paulino Alaya**  
- **Nicolas Stirparo**
  
---
