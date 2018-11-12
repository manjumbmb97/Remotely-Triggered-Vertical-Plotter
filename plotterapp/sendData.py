import serial
#import RPi.GPIO as GPIO
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
	serData = "0,16435,17126"
	ser.write(serData.encode())

	time.sleep(2)
	#rFlag = False
	#lFlag = False

	# while True:
	# 	readData = ser.read()
	# 	if str(readData.decode()) == "r":
	# 		rFlag = True
	# 	if str(readData.decode()) == "l":
	# 		lFlag = True
	# 	if rFlag and lFlag:
	# 		break
	# 	else:
	# 		continue

	# rFlag = False
	# lFlag = False

	if len(coords) > 1:
		coords[i]="0,"+coords[i]
		ser.write(coords[i].encode())
		i+=1
	else:
		print("No coords to send")
		return

	while True:
		time.sleep(2)
		if i<len(coords)-1:
			coords[i]="1,"+coords[i]
			ser.write(coords[i].encode())
			print(coords[i]+": sent")
			i+=
		else:
			print("data transmission ended")
			break
	return 1