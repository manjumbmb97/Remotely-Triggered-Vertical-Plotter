from PIL import Image
import os
basedir = os.path.abspath(os.path.dirname(__file__))

def get(filename):
	pic = Image.open(basedir + '/static/images/'+filename)

	pic_data = list(pic.getdata())

	img_col = (pic.size)[0]
	img_row = (pic.size)[1]

	txtfile = open(basedir + '/static/txt/'+filename[:-4]+".txt", "w")

	for i in range(0,img_row):
		for j in range(0,img_col):
			pixel_num = img_col*i + j
			pixel = pic_data[pixel_num]
			if pixel[3] != 0:
				txtfile.write(str(i)+" "+str(j)+"\n")

	txtfile.close()