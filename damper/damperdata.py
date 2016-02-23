import matplotlib.pyplot as plt
import csv

time=[]
vel=[]
dire=[]
pos=[]
speed=[]

with open('damperdata.csv', 'rb') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',')
	for row in spamreader:
			time.append(float(row[0]))
			vel.append(float(row[1]))
			dire.append(float(row[2]))
			speed.append(float(row[3]))
			pos.append(float(row[4]))

speedS=[]
for i in range(len(time)):
	speedS.append(speed[i]*dire[i])

f, axarr = plt.subplots(3, sharex=True) 
axarr[0].plot(time,pos,label='Angle',color='CornflowerBlue', linewidth=2)
axarr[1].plot(time,vel,label='Angular Velocity',color='LightSalmon',linewidth=2)
axarr[2].plot(time,speedS,label='MotorSpeed',color='DarkSeaGreen',linewidth=2)
axarr[0].legend()
axarr[1].legend()
axarr[2].legend()
axarr[2].set_xlabel('Time in seconds')
axarr[0].set_title('Damper')
axarr[1].set_xlim([0,12])
plt.show()