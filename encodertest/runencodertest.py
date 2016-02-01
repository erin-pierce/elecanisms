import encodertest as encoder
import time
import struct


myenc=encoder.encodertest()
read_angle=0x3FFF
read_mag=0x3FFE

# def getAngle()
while(1):
	# time.sleep(1)
	rawangle=myenc.enc_readReg(read_mag)
	# errorreg=
	angle=struct.unpack('<h',rawangle)	
	angle=int(angle[0])
	print 'Raw Value', angle
	anglemasked=angle&0x3FFF
	print 'Raw Angle', anglemasked
	error=angle&0x4000
	# print angle
	anglemasked=float(anglemasked)*(360.0/16384.0)
	print 'Angle', anglemasked
	if error >0:
		print 'ERROR'
	time.sleep(0.5)
	print '---'