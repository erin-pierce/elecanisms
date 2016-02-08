import matplotlib.pyplot as plt
import csv


time=[]
data=[]
with open('spindata.csv', 'rb') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',')
	for row in spamreader:
			time.append(float(row[0]))
			data.append(float(row[1]))


plt.plot(time,data, linewidth=3,color='CornflowerBlue')
plt.xlabel('Seconds')
plt.ylabel('Angle')
plt.show()