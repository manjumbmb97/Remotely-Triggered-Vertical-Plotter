import serial
import RPi.GPIO as GPIO
import time


ser = serial.Serial('/dev/ttyUSB0',115200)

def sendCoordinates():
	with open('plotterapp/static/txt/test.txt','r') as f:
		coords = f.read().split('\n')
	i=0
	# while True:
	# 	if i < len(coords) - 1:
	# 		print(coords[i])
	# 		i+=1
	# 	else:
	# 		print("data transmission ended")
	# 		break
	# 	time.sleep(1)

	if len(coords) > 1:
		ser.write(coords[i].encode())
		i+=1
	else:
		print("No coords to send")
		return

	while True:
		readData = ser.read()
		if int(readData.decode()) == 1:
			if i<len(coords)-1:
				ser.write(coords[i].encode())
				print(coords[i]+": sent")
				i+=1
			else:
				print("data transmission ended")
				break
		else:
			continue
	return 1