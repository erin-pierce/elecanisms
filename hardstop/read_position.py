import encodertest as encoder
import time
import struct
import csv


myenc=encoder.encodertest()
# read_angle=0x3FFF
# read_mag=0x3FFE
# read_Error=0x3FFD
# timevec=[]

# while(1):
# 	rawangle=myenc.enc_readReg(read_angle)
# 	angle=struct.unpack('<H',rawangle)
# 	angle=int(angle[0])
# 	print 'Raw Value', angle
# 	anglemasked=angle&0x3FFF
# 	print 'Raw Angle', anglemasked
# 	error=angle&0x4000
# 	anglemasked=float(anglemasked)*(360.0/16384.0)
# 	print 'Angle', anglemasked
# 	if error >0:
# 		print 'ERROR'
# 	time.sleep(0.3)
# 	print '---'

while (1):
	time.sleep(1)
	# myenc.toggle_led2()
	# print 'Toggled LED2'
	data=myenc.get_pos()
	pos=struct.unpack('<H',data)
	print 'Position', pos[0]
	data=myenc.get_angle()
	angle=struct.unpack('<H',data)
	print 'Angle', angle[0]