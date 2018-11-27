import serial
#import RPi.GPIO as GPIO
import time


ser = serial.Serial('/dev/ttyUSB0',115200)

def sendCoordinates():
	with open('plotterapp/static/txt/test.txt','r') as f:
		coords = f.read().split('\n')

	j=0
	while j<len(coords)-1:
		coords[j]="1,"+coords[j]
		j+=1

	#insert origin at top most position
	origin = "0,16435,17126"
	coords.insert(0,origin)

	rFlag = False
	lFlag = False
	i=0

	#write origin to port iff there is some point other than origin also
	if len(coords) > 2:
		ser.write(coords[i].encode())
	else:
		print("No coords to send")
		return 0

	while True:
		readData = ser.readline()
		readData = str(readData.decode()).split(',')
		sentCoords = coords[i].split(',')
		if readData[0]=='X' or readData[1]=='X':
			continue
		if (int(readData[0])-int(sentCoords[1]))<200:
			rFlag=True
		if (int(readData[1])-int(sentCoords[2]))<200:
			lFlag=True
		if rFlag and lFlag:
			i+=1
			if i<len(coords)-1:
				ser.write(coords[i].encode())
				print(coords[i]+": sent")
				continue
			else:
				print("data transmission ended")
				break
	return 1