# Monitor Serial - Sensores de Tensión y Corriente

Este proyecto incluye tres versiones de un monitor serial para sensores de tensión y corriente:

## 🎨 Versión GUI Simplificada (Recomendada)

### Características:
- **Interfaz gráfica moderna** con ventana separada
- **Tabla de datos** en tiempo real
- **Panel de control** con botones para conectar/desconectar
- **Log de actividad** en tiempo real
- **Valores actuales** mostrados en pantalla
- **Guardado automático** en archivo CSV
- **Compatible con macOS** - No requiere matplotlib

### Cómo ejecutar:

1. **Primera vez (instalación):**
   ```bash
   # Crear entorno virtual
   python3 -m venv venv
   
   # Activar entorno virtual
   source venv/bin/activate
   
   # Instalar dependencias
   pip install -r requirements.txt
   ```

2. **Ejecutar la GUI simplificada:**
   ```bash
   # Opción 1: Usar el script
   ./run_simple_gui.sh
   
   # Opción 2: Manual
   source venv/bin/activate
   python3 piserial_gui_simple.py
   ```

### Funcionalidades:
- **Conectar/Desconectar**: Botón para iniciar/detener la comunicación
- **Limpiar Datos**: Borra todos los datos y tabla
- **Tabla de datos**: Muestra los últimos 20 registros en tiempo real
- **Log de actividad**: Registra todos los eventos
- **Guardado automático**: Los datos se guardan en `sensor_data.csv`

## 📊 Versión GUI con Gráficos (Avanzada)

### Características:
- **Gráficos en tiempo real** de tensión, corriente y potencia
- **Interfaz más avanzada** con matplotlib
- **Puede tener problemas de compatibilidad** en algunos sistemas macOS

### Cómo ejecutar:
```bash
# Solo si la versión simplificada funciona bien
source venv/bin/activate
python3 piserial_gui.py
```

**Nota**: Si obtienes errores de Tcl/Tk, usa la versión simplificada.

## 📟 Versión Consola (Original)

### Características:
- Interfaz de línea de comandos
- Guardado en CSV
- Salida en consola

### Cómo ejecutar:
```bash
python3 piserial.py
```

## Configuración del Puerto

Por defecto, el programa usa el puerto `/dev/cu.usbserial-130`. Si tu dispositivo usa un puerto diferente:

1. **En las versiones GUI**: Cambia el puerto en el campo de texto antes de conectar
2. **En la versión consola**: Edita la variable `port` en el archivo `piserial.py`

## Archivos del Proyecto

- `piserial_gui_simple.py` - **Versión GUI simplificada (recomendada)**
- `piserial_gui.py` - Versión GUI con gráficos (avanzada)
- `piserial.py` - Versión de consola original
- `requirements.txt` - Dependencias necesarias
- `run_simple_gui.sh` - Script para ejecutar la GUI simplificada
- `run_gui.sh` - Script para ejecutar la GUI con gráficos
- `sensor_data.csv` - Archivo donde se guardan los datos

## Dependencias

- `pyserial` - Comunicación serial
- `tkinter` - Interfaz gráfica (incluido con Python)
- `matplotlib` - Gráficos (solo para versión avanzada)

## Solución de Problemas

### Error de Tcl/Tk en macOS:
Si obtienes el error `ImportError: Failed to load Tcl_SetVar`, usa la versión simplificada:
```bash
./run_simple_gui.sh
```

### Puerto no encontrado:
1. Verifica que el dispositivo esté conectado
2. Cambia el puerto en la interfaz
3. En macOS, los puertos suelen ser `/dev/cu.usbserial-XXXX`

## Notas

- La versión GUI simplificada es la más compatible y estable
- Los datos se guardan automáticamente en formato CSV
- El programa calcula automáticamente la potencia (tensión × corriente)
- Todas las versiones mantienen la misma funcionalidad de comunicación serial 