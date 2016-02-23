import hardstop_usb
import time
import struct
import csv

speedvec=[]
angvec=[]
timevec=[]

myusb=hardstop_usb.usbcomm()
time.sleep(2)
startt = time.time()
# while(1):
for i in range (1000):
	time.sleep(0.01)
	runtime=time.time()-startt
	timevec.append(runtime)
	ang=struct.unpack('<H',myusb.get_pos())
	angvec.append(ang[0])
	speed=struct.unpack('<H',myusb.get_speed())
	speedvec.append(speed[0])
	print ang[0];



with open('hardstopdata.csv', 'wb') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',')
    for i in range(len(timevec)):
    	spamwriter.writerow([timevec[i],angvec[i],speedvec[i]])