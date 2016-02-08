import encodertest as encoder
import time
import struct
import csv


myenc=encoder.encodertest()
read_angle=0x3FFF
read_mag=0x3FFE
read_Error=0x3FFD
timevec=[]
datavec=[]
startt = time.time()
def getAngle():
	rawangle=myenc.enc_readReg(read_angle)
	angle=struct.unpack('<H',rawangle)	
	angle=int(angle[0])
	anglemasked=angle&0x3FFF
	anglemasked=float(anglemasked)*(360.0/16384.0)
	return anglemasked


for i in range (5000):
	time.sleep(0.0005) #This helps the timesteps be more regular
	data=getAngle()
	runtime=time.time()-startt
	timevec.append(runtime)
 	datavec.append(data)

with open('spindata.csv', 'wb') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',')
    for i in range(len(timevec)):
    	spamwriter.writerow([timevec[i],datavec[i]])





