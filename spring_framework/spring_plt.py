import matplotlib.pyplot as plt
import csv

time=[]
vel=[]
dire=[]
pos=[]
speed=[]

with open('springdata.csv', 'rb') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',')
	for row in spamreader:
			time.append(float(row[0])-65)
			dire.append(float(row[1]))
			speed.append(float(row[2]))
			pos.append(float(row[3]))

speedS=[]
for i in range(len(time)):
	speedS.append(speed[i]*dire[i])

f, axarr = plt.subplots(2, sharex=True) 
axarr[0].plot(time,pos,label='Angle',color='CornflowerBlue', linewidth=2)
axarr[1].plot(time,speedS,label='Motor Speed',color='LightSalmon',linewidth=2)
axarr[0].legend()
axarr[1].legend()
axarr[1].set_xlabel('Time in seconds')
axarr[0].set_title('Spring')
axarr[1].set_xlim([0,12])
plt.show()