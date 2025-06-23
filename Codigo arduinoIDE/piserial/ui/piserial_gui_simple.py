import serial
import csv
import datetime
import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext
import threading
import time
from collections import deque

class SerialMonitorSimpleGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Monitor Serial - Sensores de Tensión y Corriente")
        self.root.geometry("800x600")
        self.root.configure(bg='#f0f0f0')
        
        # Variables
        self.ser = None
        self.is_running = False
        self.csv_filename = "sensor_data.csv"
        self.port = "/dev/cu.usbserial-130"
        
        # Datos para mostrar
        self.max_display_points = 20
        self.data_points = deque(maxlen=self.max_display_points)
        
        self.setup_gui()
        
    def setup_gui(self):
        # Frame principal
        main_frame = ttk.Frame(self.root, padding="10")
        main_frame.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
        # Configurar grid
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)
        main_frame.columnconfigure(1, weight=1)
        main_frame.rowconfigure(2, weight=1)
        
        # Panel de control
        control_frame = ttk.LabelFrame(main_frame, text="Control de Conexión", padding="10")
        control_frame.grid(row=0, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        
        # Puerto serie
        ttk.Label(control_frame, text="Puerto:").grid(row=0, column=0, sticky=tk.W, padx=(0, 5))
        self.port_var = tk.StringVar(value=self.port)
        port_entry = ttk.Entry(control_frame, textvariable=self.port_var, width=20)
        port_entry.grid(row=0, column=1, sticky=tk.W, padx=(0, 10))
        
        # Botones
        self.connect_btn = ttk.Button(control_frame, text="Conectar", command=self.toggle_connection)
        self.connect_btn.grid(row=0, column=2, padx=(0, 5))
        
        self.clear_btn = ttk.Button(control_frame, text="Limpiar Datos", command=self.clear_data)
        self.clear_btn.grid(row=0, column=3, padx=(0, 5))
        
        # Estado de conexión
        self.status_var = tk.StringVar(value="Desconectado")
        self.status_label = ttk.Label(control_frame, textvariable=self.status_var, 
                                     foreground="red", font=("Arial", 10, "bold"))
        self.status_label.grid(row=0, column=4, padx=(10, 0))
        
        # Panel de valores actuales
        values_frame = ttk.LabelFrame(main_frame, text="Valores Actuales", padding="10")
        values_frame.grid(row=1, column=0, columnspan=2, sticky=(tk.W, tk.E), pady=(0, 10))
        
        # Tensión
        ttk.Label(values_frame, text="Tensión (V):", font=("Arial", 12, "bold")).grid(row=0, column=0, sticky=tk.W, padx=(0, 10))
        self.tension_var = tk.StringVar(value="--")
        tension_label = ttk.Label(values_frame, textvariable=self.tension_var, 
                                 font=("Arial", 14), foreground="blue")
        tension_label.grid(row=0, column=1, sticky=tk.W, padx=(0, 30))
        
        # Corriente
        ttk.Label(values_frame, text="Corriente (A):", font=("Arial", 12, "bold")).grid(row=0, column=2, sticky=tk.W, padx=(0, 10))
        self.corriente_var = tk.StringVar(value="--")
        corriente_label = ttk.Label(values_frame, textvariable=self.corriente_var, 
                                   font=("Arial", 14), foreground="green")
        corriente_label.grid(row=0, column=3, sticky=tk.W, padx=(0, 30))
        
        # Potencia
        ttk.Label(values_frame, text="Potencia (W):", font=("Arial", 12, "bold")).grid(row=0, column=4, sticky=tk.W, padx=(0, 10))
        self.potencia_var = tk.StringVar(value="--")
        potencia_label = ttk.Label(values_frame, textvariable=self.potencia_var, 
                                  font=("Arial", 14), foreground="red")
        potencia_label.grid(row=0, column=5, sticky=tk.W)
        
        # Panel de datos recientes
        data_frame = ttk.LabelFrame(main_frame, text="Datos Recientes", padding="10")
        data_frame.grid(row=2, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 10))
        data_frame.columnconfigure(0, weight=1)
        data_frame.rowconfigure(0, weight=1)
        
        # Treeview para mostrar datos
        columns = ('timestamp', 'tension', 'corriente', 'potencia')
        self.tree = ttk.Treeview(data_frame, columns=columns, show='headings', height=10)
        
        # Configurar columnas
        self.tree.heading('timestamp', text='Hora')
        self.tree.heading('tension', text='Tensión (V)')
        self.tree.heading('corriente', text='Corriente (A)')
        self.tree.heading('potencia', text='Potencia (W)')
        
        self.tree.column('timestamp', width=150)
        self.tree.column('tension', width=100)
        self.tree.column('corriente', width=100)
        self.tree.column('potencia', width=100)
        
        # Scrollbar para el treeview
        scrollbar = ttk.Scrollbar(data_frame, orient=tk.VERTICAL, command=self.tree.yview)
        self.tree.configure(yscrollcommand=scrollbar.set)
        
        self.tree.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        scrollbar.grid(row=0, column=1, sticky=(tk.N, tk.S))
        
        # Panel de logs
        log_frame = ttk.LabelFrame(main_frame, text="Log de Actividad", padding="10")
        log_frame.grid(row=3, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S))
        log_frame.columnconfigure(0, weight=1)
        log_frame.rowconfigure(0, weight=1)
        
        self.log_text = scrolledtext.ScrolledText(log_frame, height=8, width=80)
        self.log_text.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
    def log_message(self, message):
        timestamp = datetime.datetime.now().strftime("%H:%M:%S")
        self.log_text.insert(tk.END, f"[{timestamp}] {message}\n")
        self.log_text.see(tk.END)
        
    def toggle_connection(self):
        if not self.is_running:
            self.connect()
        else:
            self.disconnect()
            
    def connect(self):
        try:
            self.port = self.port_var.get()
            self.ser = serial.Serial(self.port, 9600, timeout=1)
            self.is_running = True
            
            # Actualizar interfaz
            self.status_var.set("Conectado")
            self.status_label.configure(foreground="green")
            self.connect_btn.configure(text="Desconectar")
            self.port_var.set(self.port)
            
            self.log_message(f"Conectado al puerto {self.port}")
            
            # Iniciar thread de lectura
            self.read_thread = threading.Thread(target=self.read_serial_data, daemon=True)
            self.read_thread.start()
            
        except serial.SerialException as e:
            messagebox.showerror("Error", f"No se pudo abrir el puerto {self.port}: {e}")
            self.log_message(f"Error de conexión: {e}")
            
    def disconnect(self):
        self.is_running = False
        if self.ser and self.ser.is_open:
            self.ser.close()
            
        # Actualizar interfaz
        self.status_var.set("Desconectado")
        self.status_label.configure(foreground="red")
        self.connect_btn.configure(text="Conectar")
        
        self.log_message("Desconectado del puerto serie")
        
    def read_serial_data(self):
        # Crear archivo CSV
        with open(self.csv_filename, 'w', newline='') as csvfile:
            csv_writer = csv.writer(csvfile)
            csv_writer.writerow(['Timestamp', 'Tension', 'Corriente', 'Potencia'])
            
            while self.is_running:
                try:
                    # Leer tensión
                    line_tension = self.ser.readline()
                    if not line_tension:
                        continue
                        
                    # Leer corriente
                    line_corriente = self.ser.readline()
                    if not line_corriente:
                        continue
                        
                    # Decodificar datos
                    tension_str = line_tension.decode('utf-8').strip()
                    corriente_str = line_corriente.decode('utf-8').strip()
                    
                    try:
                        tension_value = float(tension_str)
                        corriente_value = float(corriente_str)
                        potencia_value = tension_value * corriente_value
                        
                        # Actualizar valores en la GUI
                        self.root.after(0, self.update_values, tension_value, corriente_value, potencia_value)
                        
                        # Agregar a tabla de datos
                        timestamp = datetime.datetime.now()
                        self.root.after(0, self.add_data_point, timestamp, tension_value, corriente_value, potencia_value)
                        
                        # Guardar en CSV
                        csv_writer.writerow([timestamp.isoformat(), tension_value, corriente_value, potencia_value])
                        csvfile.flush()  # Forzar escritura inmediata
                        
                        self.root.after(0, self.log_message, 
                                      f"Tensión: {tension_value:.2f}V, Corriente: {corriente_value:.3f}A, Potencia: {potencia_value:.2f}W")
                        
                    except ValueError:
                        self.root.after(0, self.log_message, 
                                      f"Error: Valores inválidos - Tensión: '{tension_str}', Corriente: '{corriente_str}'")
                        
                except Exception as e:
                    self.root.after(0, self.log_message, f"Error de lectura: {e}")
                    break
                    
    def update_values(self, tension, corriente, potencia):
        self.tension_var.set(f"{tension:.2f}")
        self.corriente_var.set(f"{corriente:.3f}")
        self.potencia_var.set(f"{potencia:.2f}")
        
    def add_data_point(self, timestamp, tension, corriente, potencia):
        # Agregar a la tabla
        time_str = timestamp.strftime("%H:%M:%S")
        self.tree.insert('', 0, values=(time_str, f"{tension:.2f}", f"{corriente:.3f}", f"{potencia:.2f}"))
        
        # Mantener solo los últimos 20 registros
        if len(self.tree.get_children()) > self.max_display_points:
            # Eliminar el último elemento
            children = self.tree.get_children()
            if children:
                self.tree.delete(children[-1])
            
    def clear_data(self):
        # Limpiar tabla
        for item in self.tree.get_children():
            self.tree.delete(item)
        
        # Limpiar log
        self.log_text.delete(1.0, tk.END)
        
        # Resetear valores
        self.tension_var.set("--")
        self.corriente_var.set("--")
        self.potencia_var.set("--")
        
        self.log_message("Datos limpiados")
        
    def on_closing(self):
        if self.is_running:
            self.disconnect()
        self.root.destroy()

def main():
    root = tk.Tk()
    app = SerialMonitorSimpleGUI(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()

if __name__ == "__main__":
    main() 