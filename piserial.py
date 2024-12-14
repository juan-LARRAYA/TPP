import serial
port = "/dev/cu.usbserial-140"

ser = serial.Serial(port, 115200, timeout=0)
while True:
    data = ser.readline()
    data_sensor = data.decode('utf-8')
    print(data_sensor)



    