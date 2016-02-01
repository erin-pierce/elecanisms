#include "AS5048_erin.h"
#include "../hellousb/usb.h"
#include "pins.h"
#include "common.h"

_SPI spi1;
_AS5048 as5048;
uint16_t val1, val2;

int16_t main(void) {
    init_clock();
    init_uart();
    val1=6;
    val2=7;
    as5048_init();
    init_as5048(&as5048,&D[3]);
    as5048_getRawAngle(&as5048);
    (int16_t)val1=as5048->RawAngle

    InitUSB();                              // initialize the USB registers and serial interface engine
    while (USB_USWSTAT!=CONFIG_STATE) {     // while the peripheral is not configured...
        ServiceUSB();                       // ...service USB requests
    }
    while (1) {
        ServiceUSB();                       // service any pending USB requests
    }
}