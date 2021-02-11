import serial
import io
import time

ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 9600
ser.parity = serial.PARITY_EVEN
ser.stopbits = serial.STOPBITS_TWO
ser.bytesize = serial.EIGHTBITS
print(ser.name)

while 1:
    s = ser.readline()
    print(s.decode("ascii"))
    time.sleep(0.5)

ser.close()