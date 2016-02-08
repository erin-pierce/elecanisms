import encodertest as encoder
import time
import struct
import csv


myenc=encoder.encodertest()
read_angle=0x3FFF
read_mag=0x3FFE
read_Error=0x3FFD
response='1'
real_angle_vec=[]
measured_angle_vec=[]

def getAngle():
	rawangle=myenc.enc_readReg(read_mag)
	angle=struct.unpack('<h',rawangle)	
	angle=int(angle[0])
	anglemasked=angle&0x3FFF
	anglemasked=float(anglemasked)*(360.0/16384.0)
	return anglemasked

while str(response) !='0':
	response = input('Enter 1 to take data, or 0 to quit:   ')

	if str(response)=='1':
		datavec=[]
		for i in range(100):
			datavec.append(getAngle())
		angle=sum(datavec)/len(datavec)
		print datavec
		print 'Angle: ', angle
		measured_angle_vec.append(angle)

		real_angle= input('Enter actual angle:   ')
		real_angle_vec.append(float(real_angle))

with open('caldata.csv', 'wb') as csvfile:
    spamwriter = csv.writer(csvfile, delimiter=',')
    for i in range(len(real_angle_vec)):
    	spamwriter.writerow([real_angle_vec[i],measured_angle_vec[i]])











