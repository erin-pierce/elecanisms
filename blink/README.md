These are instructions for running 'blink' on OSX El Cap 10.11.2

You need to have python, pip, and homebrew installed already.
You will also need some python usb libraries, libusb 1.0
>> brew install libusb -compat
and pyusb
>> sudo pip install pyusb

With this set up, we need to install the complier for the 16 bit PIC microcontroller.  Download MPLAB XC16 for Mac from 
http://www.microchip.com/pagehandler/en_us/devtools/mplabxc/

Go through the install process.  I opted to modify my path variable.

Next, we need Scons.

>> brew install scons

To get the blink script, you need to fork/clone my elecanisms git repo 

https://github.com/erin-pierce/elecanisms

Now navigate into the blink folder and run
>> scons

This should genearate a hex file for us to upload.

Now navigate into the 'site_scons' folder and run
>> python bootloadergui.py

This should bring up a gui to program the board.  Plug in your board, and put it in bootloader mode by hitting the red button (reset) and then holding the black button next to it (stay in bootloader mode).

Once the board is connected, open the file menu (upper left) and import blink.hex from the blink folder.  Now select 'Write'.

Once the program has been written, select 'Disconnect/Run' to allow the code to execute.

The red and green LEDs should toggle every 0.75 seconds.  The blue LED lights up when the button SW2 is pressed.  SW3 does nothing.