import matplotlib.pyplot as plt
import csv

time=[]
defl=[]
dire=[]
pos=[]
speed=[]
defdir=[]

with open('nodedata1.csv', 'rb') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',')
	for row in spamreader:
			time.append(float(row[0]))
			defl.append(float(row[1]))
			dire.append(float(row[2]))
			speed.append(float(row[3]))
			pos.append(float(row[4]))
			defdir.append(float(row[5]))

speedS=[]
defs=[]
for i in range(len(time)):
	speedS.append(speed[i]*dire[i])
	defs.append(defl[i]*defdir[i])

f, axarr = plt.subplots(3, sharex=True) 
axarr[0].plot(time,pos,label='Angle',color='CornflowerBlue', linewidth=2)
axarr[1].plot(time,speedS,label='MotorSpeed',color='LightSalmon',linewidth=2)
axarr[2].plot(time,defs,label='Deflection',color='DarkSeaGreen',linewidth=2)
axarr[0].legend()
axarr[1].legend()
axarr[2].legend()
axarr[2].set_xlabel('Time in seconds')
axarr[0].set_title('Multiple Nodes')
plt.show()