import serial
#import RPi.GPIO as GPIO
from serial.tools import list_ports



ser = serial.Serial('/dev/ttyUSB0',115200)

def sendCoordinates(coords):
	ser.write(coords.encode())