import usb.core
import time

class usbcomm:

    def __init__(self):
        self.TOGGLE_LED1 = 1
        self.TOGGLE_LED2 = 2
        self.TOGGLE_LED3 = 3
        self.SEND_POS = 4
        self.SEND_ANG = 5
        self.SEND_MTRSPD= 6
        self.dev = usb.core.find(idVendor = 0x6666, idProduct = 0x0003)
        if self.dev is None:
            raise ValueError('no USB device found matching idVendor = 0x6666 and idProduct = 0x0003')
        self.dev.set_configuration()

    def close(self):
        self.dev = None

    def toggle_led1(self):
        try:
            self.dev.ctrl_transfer(0x40, self.TOGGLE_LED1)
        except usb.core.USBError:
            print "Could not send TOGGLE_LED1 vendor request."

    def toggle_led2(self):
        try:
            self.dev.ctrl_transfer(0x40, self.TOGGLE_LED2)
        except usb.core.USBError:
            print "Could not send TOGGLE_LED2 vendor request."

    def toggle_led3(self):
        try:
            self.dev.ctrl_transfer(0x40, self.TOGGLE_LED3)
        except usb.core.USBError:
            print "Could not send TOGGLE_LED3 vendor request."

    def enc_readReg(self, address):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.ENC_READ_REG, address, 0, 2)
        except usb.core.USBError:
            print "Could not send ENC_READ_REG vendor request."
        else:
            return ret

    def get_pos(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.SEND_POS, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send SEND_POS vendor request."
        else:
            return ret 
    def get_angle(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.SEND_ANG, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send SEND_ANG vendor request."
        else:
            return ret 
    def get_speed(self):
        try:
            ret = self.dev.ctrl_transfer(0xC0, self.SEND_MTRSPD, 0, 0, 2)
        except usb.core.USBError:
            print "Could not send SEND_MTRSPD vendor request."
        else:
            return ret 
