import mulitnode_usb
import time
import struct
import csv

defvec=[]
speedvec=[]
angvec=[]
timevec=[]
dirvec=[]
defdirvec=[]
##Velocity tends to be 100-600
myusb=mulitnode_usb.usbcomm()
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
	defl=struct.unpack('<h',myusb.get_deflection())
	defvec.append(defl[0])
	speed=struct.unpack('<H',myusb.get_speed())
	speedvec.append(speed[0])
	direct=myusb.get_direction()[0]
	if direct==0:
		direct=-1
	dirvec.append(direct)
	defs=myusb.get_defsign()[0]
	if defs==0:
		defs=-1
	defdirvec.append(defs)
	print 'DEF', defl[0],' // Speed', speed[0] , ' // Dir', direct



with open('nodedata.csv', 'wb') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',')
    for i in range(len(timevec)):
    	spamwriter.writerow([timevec[i],defvec[i],dirvec[i],speedvec[i],angvec[i],defdirvec[i]])


