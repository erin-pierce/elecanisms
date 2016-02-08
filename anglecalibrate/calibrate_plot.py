import matplotlib.pyplot as plt
import csv


actual=[]
measured=[]
with open('caldata.csv', 'rb') as csvfile:
	spamreader = csv.reader(csvfile, delimiter=',')
	for row in spamreader:
			actual.append(float(row[0]))
			measured.append(float(row[1]))


plt.plot(actual,measured, linewidth=3,color='IndianRed')
plt.xlabel('Actual Angle')
plt.ylabel('Measured Angle')
plt.show()