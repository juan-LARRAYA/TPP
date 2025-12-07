import serial
import csv
import datetime
import tkinter as tk
from tkinter import ttk, messagebox, scrolledtext
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import threading
import time
from collections import deque
import os

class SerialMonitorGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Monitor Serial - Sensores de Tensión y Corriente")
        self.root.geometry("1200x800")
        self.root.configure(bg='#f0f0f0')
        
        # Variables
        self.ser = None
        self.is_running = False
        self.csv_filename = "sensor_data.csv"
        self.port = "/dev/cu.usbserial-130"
        
        # Datos para gráficos
        self.max_points = 100
        self.timestamps = deque(maxlen=self.max_points)
        self.tension_data = deque(maxlen=self.max_points)
        self.corriente_data = deque(maxlen=self.max_points)
        self.potencia_data = deque(maxlen=self.max_points)
        
        self.setup_gui()
        self.setup_plot()
        
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
        
        # Panel de gráficos
        plot_frame = ttk.LabelFrame(main_frame, text="Gráficos en Tiempo Real", padding="10")
        plot_frame.grid(row=2, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S), pady=(0, 10))
        plot_frame.columnconfigure(0, weight=1)
        plot_frame.rowconfigure(0, weight=1)
        
        # Panel de logs
        log_frame = ttk.LabelFrame(main_frame, text="Log de Actividad", padding="10")
        log_frame.grid(row=3, column=0, columnspan=2, sticky=(tk.W, tk.E, tk.N, tk.S))
        log_frame.columnconfigure(0, weight=1)
        log_frame.rowconfigure(0, weight=1)
        
        self.log_text = scrolledtext.ScrolledText(log_frame, height=8, width=80)
        self.log_text.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
        
    def setup_plot(self):
        # Crear figura con subplots
        self.fig = Figure(figsize=(12, 6), dpi=100)
        
        # Subplot para tensión y corriente
        self.ax1 = self.fig.add_subplot(211)
        self.ax1.set_title('Tensión y Corriente vs Tiempo', fontsize=12, fontweight='bold')
        self.ax1.set_ylabel('Valor')
        self.ax1.grid(True, alpha=0.3)
        
        # Subplot para potencia
        self.ax2 = self.fig.add_subplot(212)
        self.ax2.set_title('Potencia vs Tiempo', fontsize=12, fontweight='bold')
        self.ax2.set_ylabel('Potencia (W)')
        self.ax2.set_xlabel('Tiempo')
        self.ax2.grid(True, alpha=0.3)
        
        # Líneas de los gráficos
        self.line_tension, = self.ax1.plot([], [], 'b-', label='Tensión (V)', linewidth=2)
        self.line_corriente, = self.ax1.plot([], [], 'g-', label='Corriente (A)', linewidth=2)
        self.line_potencia, = self.ax2.plot([], [], 'r-', label='Potencia (W)', linewidth=2)
        
        self.ax1.legend()
        self.ax2.legend()
        
        # Canvas para tkinter
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.root)
        self.canvas_widget = self.canvas.get_tk_widget()
        
        # Encontrar el frame de gráficos y agregar el canvas
        for widget in self.root.winfo_children():
            if isinstance(widget, ttk.Frame):
                for child in widget.winfo_children():
                    if isinstance(child, ttk.LabelFrame) and "Gráficos" in child.cget("text"):
                        child.columnconfigure(0, weight=1)
                        child.rowconfigure(0, weight=1)
                        self.canvas_widget.grid(row=0, column=0, sticky=(tk.W, tk.E, tk.N, tk.S))
                        break
        
        self.fig.tight_layout()
        
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
                        
                        # Agregar a datos de gráfico
                        timestamp = datetime.datetime.now()
                        self.root.after(0, self.update_plot, timestamp, tension_value, corriente_value, potencia_value)
                        
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
        
    def update_plot(self, timestamp, tension, corriente, potencia):
        # Agregar datos
        self.timestamps.append(timestamp)
        self.tension_data.append(tension)
        self.corriente_data.append(corriente)
        self.potencia_data.append(potencia)
        
        # Actualizar gráficos
        if len(self.timestamps) > 1:
            # Convertir timestamps a números para el eje X
            time_nums = [(t - self.timestamps[0]).total_seconds() for t in self.timestamps]
            
            # Actualizar líneas
            self.line_tension.set_data(time_nums, list(self.tension_data))
            self.line_corriente.set_data(time_nums, list(self.corriente_data))
            self.line_potencia.set_data(time_nums, list(self.potencia_data))
            
            # Ajustar límites de ejes
            self.ax1.relim()
            self.ax1.autoscale_view()
            self.ax2.relim()
            self.ax2.autoscale_view()
            
            # Redibujar
            self.canvas.draw()
            
    def clear_data(self):
        # Limpiar datos
        self.timestamps.clear()
        self.tension_data.clear()
        self.corriente_data.clear()
        self.potencia_data.clear()
        
        # Limpiar gráficos
        self.line_tension.set_data([], [])
        self.line_corriente.set_data([], [])
        self.line_potencia.set_data([], [])
        
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
    app = SerialMonitorGUI(root)
    root.protocol("WM_DELETE_WINDOW", app.on_closing)
    root.mainloop()

if __name__ == "__main__":
    main() 