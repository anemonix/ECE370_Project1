import serial
import time

ser = serial.Serial('/dev/ttyS0', baudrate=9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE,bytesize=serial.EIGHTBITS)

ser.isOpen()

print("Connected on: " + ser.portstr)
while True:
    inp = raw_input("Enter angle for servo: ")
    
    if float(inp) >= 0:
        sign = 1
    else:
        sign = 0
    
    divPart = abs(int(float(inp))) / 256
    modPart = abs(int(float(inp))) % 256

    checkSum = ((divPart + modPart - sign) % 255)
    ser.write(str(chr(0x00) + chr(0x00) + chr(sign) + chr(divPart) + chr(modPart) + chr(checkSum)))
