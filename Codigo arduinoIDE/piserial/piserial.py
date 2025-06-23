import serial
port = "/dev/cu.usbserial-130"
n = 0
ser = serial.Serial(port, 9600, timeout=100)
while True:
    data = ser.readline()
    try:
        data_sensor = data.decode('utf-8')
        print(data_sensor)
        n += 1
        if n == 4:
            print("-----------------------------")
            n = 0
    except UnicodeDecodeError:
        print("Error decoding data")






    