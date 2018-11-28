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

	rFlag = False
	lFlag = False
	i=0

	#write origin to port iff there is some point other than origin also
	# if len(coords) > 1:
	# 	ser.write(coords[i].encode())
	# else:
	# 	print("No coords to send")
	# 	return 0

	while True:
		if i<len(coords)-1:
			ser.write(coords[i].encode())
			data=coords[i].split(",")
			print("x:"+((data[1]/42.8)*25)+",y:"+((data[2]/44.16)*25)+": sent")
			i+=1
		else:
			print("data transmission ended")
			break
		time.sleep(2)
	return 1