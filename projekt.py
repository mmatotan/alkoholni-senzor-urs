import serial
import datetime
import csv
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import dateutil

# Specifikacija USARTa
ser = serial.Serial('/dev/ttyUSB0')
ser.baudrate = 9600
ser.parity = serial.PARITY_EVEN
ser.stopbits = serial.STOPBITS_TWO
ser.bytesize = serial.EIGHTBITS

date = datetime.date.today()
filename = f'{date.strftime("%d-%m-%Y")}.csv'
csvFile = open(filename, "a+")
fileWriter = csv.writer(csvFile)

# Za smanjenje nepotrebnih podataka
last_measure = -1

while 1:
    rec = ser.readline()
    rec = int(rec.decode("ascii"))
    rec = rec
    if(rec == last_measure):
        continue
    output_time = f'{datetime.datetime.now().strftime("%H:%M:%S:%f")[:-4]}'
    print(f'{output_time}\t{rec}')
    fileWriter.writerow([output_time] + [rec])
    last_measure = rec

ser.close()