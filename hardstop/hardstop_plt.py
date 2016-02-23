import matplotlib.pyplot as plt
import csv

time=[]
pos=[]
speed=[]

with open('hardstopdata1.csv', 'rb') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',')
	for row in spamreader:
			time.append(float(row[0]))
			speed.append(float(row[2]))
			pos.append(float(row[1]))


f, axarr = plt.subplots(2, sharex=True) 
axarr[0].plot(time,pos,label='Angle',color='CornflowerBlue', linewidth=2)
axarr[1].plot(time,speed,'x-',label='MotorSpeed',color='LightSalmon',linewidth=2)
axarr[1].legend(loc='upper left')
axarr[0].legend(loc='upper left')
axarr[1].set_xlabel('Time in seconds')
axarr[0].set_title('Wall')
axarr[1].set_xlim([0,9])
plt.legend()
plt.show()