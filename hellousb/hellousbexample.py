import hellousb as usblib
import time

myusb=usblib.hellousb()

while(1):
	time.sleep(1)
	# myusb.set_vals(4,5)
	time.sleep(0.5)
	vals=myusb.get_vals()
	print vals
	print 'loop'
