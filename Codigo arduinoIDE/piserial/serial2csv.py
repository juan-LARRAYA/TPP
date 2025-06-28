import serial
import csv
import datetime
import os

port = "/dev/cu.usbserial-130"

# Función para encontrar el siguiente número de archivo disponible
def get_next_filename():
    counter = 0
    while True:
        filename = f"sensor_data_{counter}.csv"
        if not os.path.exists(filename):
            return filename
        counter += 1

csv_filename = get_next_filename()

try:
    # Configurar y abrir el puerto serie
    ser = serial.Serial(port, 9600, timeout=1)
    print(f"Escuchando en el puerto {port}...")

    # Abrir el archivo CSV para escribir los datos
    with open(csv_filename, 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        
        
        # Escribir la fila de encabezado para Tensión y Corriente
        csv_writer.writerow(['Timestamp', 'Tension', 'Corriente','totalizado'])
        print(f"Guardando datos en {csv_filename}")

        while True:
            # Leer la línea de Tensión
            line_tension = ser.readline()
            if not line_tension:
                continue  # Esperando datos...

            # Leer la línea de Corriente
            line_corriente = ser.readline()
            if not line_corriente:
                print("Advertencia: No se recibió el valor de corriente. Omitiendo.")
                continue

            # Leer la línea de totalizado
            linea_totalizado = ser.readline()
            if not linea_totalizado:
                print("Advertencia: No se recibió el valor de totalizado. Omitiendo.")
                continue

            # Decodificar y limpiar los datos, manejando posibles errores de decodificación
            try:
                tension_str = line_tension.decode('utf-8').strip()
            except UnicodeDecodeError:
                print("Advertencia: No se pudo decodificar la línea de tensión. Omitiendo.")
                continue
            try:
                corriente_str = line_corriente.decode('utf-8').strip()
            except UnicodeDecodeError:
                print("Advertencia: No se pudo decodificar la línea de corriente. Omitiendo.")
                continue
            try:
                totalizado_str = linea_totalizado.decode('utf-8').strip()
            except UnicodeDecodeError:
                print("Advertencia: No se pudo decodificar la línea de totalizado. Omitiendo.")
                continue

            print(f"Recibido: Tensión={tension_str}, Corriente={corriente_str}, Totalizado={totalizado_str}")

            # Intentar convertir los datos a números
            try:
                tension_value = float(tension_str)
                corriente_value = float(corriente_str)
                totalizado_value = int(totalizado_str)
                
                # Obtener la fecha y hora actual
                timestamp = datetime.datetime.now().isoformat()
                
                # Escribir la fecha/hora y los valores en el archivo CSV
                csv_writer.writerow([timestamp, tension_value, corriente_value, totalizado_value])
            except ValueError:
                # Se activa si alguno de los valores no es un número válido
                print(f"Advertencia: No se pudieron convertir los valores. Tensión='{tension_str}', Corriente='{corriente_str}', Totalizado='{totalizado_str}'. Omitiendo.")

except serial.SerialException as e:
    print(f"Error: No se pudo abrir el puerto serie {port}. {e}")
except KeyboardInterrupt:
    print("\nSaliendo del programa.")
finally:
    if 'ser' in locals() and ser.is_open:
        ser.close()
        print("Puerto serie cerrado.")
