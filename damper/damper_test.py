import damper_usb
import time
import struct
import csv
velvec=[]
dirvec=[]
speedvec=[]
angvec=[]
timevec=[]
##Velocity tends to be 100-600
myusb=damper_usb.usbcomm()
time.sleep(2)
myusb.set_scale(15)
startt = time.time()
# while(1):
for i in range (1000):
	time.sleep(0.01)
	runtime=time.time()-startt
	timevec.append(runtime)
	ang=struct.unpack('<H',myusb.get_angle())
	angvec.append(ang[0])
	vel=struct.unpack('<h',myusb.get_velocity())
	velvec.append(vel[0])
	speed=struct.unpack('<H',myusb.get_speed())
	speedvec.append(speed[0])
	print 'Velocity', vel[0],' // Speed', speed[0]
	direct=myusb.get_direction()[0]
	if direct==0:
		direct=-1
	dirvec.append(direct)


with open('damperdata.csv', 'wb') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',')
    for i in range(len(timevec)):
    	spamwriter.writerow([timevec[i],velvec[i],dirvec[i],speedvec[i],angvec[i]])