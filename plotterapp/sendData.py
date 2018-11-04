import serial
#import RPi.GPIO as GPIO
from serial.tools import list_ports

arduino_ports = [
    p.device
    for p in serial.tools.list_ports.comports()
    if 'Arduino' in p.description
]

if not arduino_ports:
    raise IOError("No Arduino found")
if len(arduino_ports) > 1:
    warnings.warn('Multiple Arduinos found - using the first')

ser = serial.Serial(arduino_ports[0])

def sendCoordinates(coords):
	while True:
		try:
			incoming_data = ser.read() 
			if incoming_data == '1':
				ser.write(coords)
			elif incoming_data == '0':
				pass
		except KeyboardInterrupt:
			GPIO.cleanup()