import spring_usb
import time
import struct
import csv
velvec=[]
dirvec=[]
speedvec=[]
angvec=[]
timevec=[]
##Velocity tends to be 100-600
myusb=spring_usb.usbcomm()
time.sleep(2)
myusb.set_pscale(5)
startt = time.time()
# while(1):
for i in range (10000):
	time.sleep(0.01)
	runtime=time.time()-startt
	timevec.append(runtime)
	ang=struct.unpack('<H',myusb.get_angle())
	angvec.append(ang[0])
	speed=struct.unpack('<H',myusb.get_speed())
	speedvec.append(speed[0])
	print 'Angle', ang[0],' // Speed', speed[0]
	direct=myusb.get_direction()[0]
	if direct==0:
		direct=-1
	dirvec.append(direct)


with open('springdata.csv', 'wb') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',')
    for i in range(len(timevec)):
    	spamwriter.writerow([timevec[i],dirvec[i],speedvec[i],angvec[i]])