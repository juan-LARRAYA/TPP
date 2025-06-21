import serial
port = "/dev/cu.usbserial-130"

ser = serial.Serial(port, 9600, timeout=100)
while True:
    data = ser.readline()
    data_sensor = data.decode('utf-8')
    print(data_sensor)





    