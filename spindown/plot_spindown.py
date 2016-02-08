import matplotlib.pyplot as plt
import csv


def Shift_Angle(time,angle):
	newAngle=[]
	newTime=[] 
	loop=0
	for i in range(len(angle)-1):

		angdiff=angle[i+1]-angle[i]
		newAngle.append(angle[i]+(loop*360))
		newTime.append(time[i])

		if angdiff <-180: #It looped around
			loop=loop+1
	return newTime,newAngle

def Calc_Velocity(time,angle):
	newTime=[]
	vel=[]
	for i in range(len(angle)-1):
		timediff=time[i+1]-time[i]
		angdiff=angle[i+1]-angle[i]
	
		if angdiff > 0: ## Two indentical measurments make it look bad
			if angdiff/timediff <100000: ## As do absurdly high data points
				newTime.append(time[i])
				vel.append(angdiff/timediff)
	return newTime, vel

def Clean_Velocity(time,vel):
	newTime=[]
	newVel=[]
	i=0
	num=4
	while i< len(vel):
		avg=sum(vel[i:i+num])/num
		newVel.append(avg)
		newTime.append(time2[i+num/2])
		i=i+num
	return newTime, newVel


time=[]
data=[]

with open('spindata_good3.csv', 'rb') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',')
	for row in spamreader:
			time.append(float(row[0]))
			data.append(float(row[1]))


time1,angle=Shift_Angle(time,data)
time2,vel=Calc_Velocity(time1,angle)
timeclean,velclean=Clean_Velocity(time2,vel)

### Plot Angle
# plt.plot(time1,angle,linewidth=3,color='CornflowerBlue')

# plt.xlabel('Seconds')
# plt.ylabel('Angle')
# plt.show()


### Plot Velocity
plt.plot(time2,vel,color='CornflowerBlue')
plt.plot(timeclean,velclean,color='LightSalmon')

plt.xlabel('Seconds')
plt.ylabel('Speed (deg/sec)')
plt.show()