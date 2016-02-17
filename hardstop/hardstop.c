#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "oc.h"
#include "usb.h"
#include "pin.h"
#include "timer.h"
#include "spi.h"
#include "md.h"

#define TOGGLE_LED1         1
#define TOGGLE_LED2         2
#define READ_SW1            3
#define ENC_WRITE_REG       4
#define ENC_READ_REG        5
#define TOGGLE_LED3         8 
#define READ_SW2            9
#define READ_SW3            10
#define SEND_POS            11
#define SEND_ANG            12

#define REG_MAG_ADDR        0x3FFE

uint8_t direction = 1;

uint16_t speed=0;
WORD position;

WORD read_angle;
WORD angle_now;
WORD angle_prev;
WORD zeropt;

_PIN *ENC_SCK, *ENC_MISO, *ENC_MOSI;
_PIN *ENC_NCS;

WORD enc_readReg(WORD address) {
    WORD cmd, result;
    cmd.w = 0x4000|address.w; //set 2nd MSB to 1 for a read
    cmd.w |= parity(cmd.w)<<15; //calculate even parity for

    pin_clear(ENC_NCS);
    spi_transfer(&spi1, cmd.b[1]);
    spi_transfer(&spi1, cmd.b[0]);
    pin_set(ENC_NCS);

    pin_clear(ENC_NCS);
    result.b[1] = spi_transfer(&spi1, 0);
    result.b[0] = spi_transfer(&spi1, 0);
    pin_set(ENC_NCS);
    return result;
}

WORD angle_init(uint8_t *loop){
    // Start with joystick all the way right, and sweep 
    // slowly till green LED lights up
    uint8_t uncal=1;
    WORD zeropt;
    WORD cal_angle;
    led_on(&led2);
    led_off(&led3);
    zeropt=enc_readReg(read_angle);

    while (uncal==1){
        cal_angle= enc_readReg(read_angle);
        if (cal_angle.w <100){
            *loop=1;
            led_on(&led3);
            led_off(&led2);
            uncal=0;
        }
    }
    return zeropt;
}


WORD mask_angle(WORD raw_angle){
    WORD masked;
    masked.w=raw_angle.w & 0x3FFF;
    return masked;
}

WORD convert_Angle(WORD data_prev, WORD data_now, uint8_t *loop){
    if (data_prev.i-data_now.i > 1000){
        // led_on(&led3);
        *loop=*loop+1;

    }
    if (data_now.i-data_prev.i > 1000){
        // led_off(&led3);
        if (*loop !=0){*loop=*loop-1;}
    }


    data_now.w=data_now.w+(16384* *loop);

    return data_now;
}


WORD hardstop(WORD position){

    if (position.w > 17000){
        speed = 50000;
        direction = 1;
        led_on(&led3);
            }

    if (position.w < 17000){
        speed = 0;
        direction = 0;
        led_off(&led3);
        }

            // direction = !direction;

        md_speed(&md2, speed);
        md_direction(&md2, direction);

}

void VendorRequests(void) {
    WORD32 address;
    WORD result;

    switch (USB_setup.bRequest) {
        case TOGGLE_LED1:
            led_toggle(&led1);
            BD[EP0IN].bytecount = 0;         // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case TOGGLE_LED2:
            led_toggle(&led2);
            BD[EP0IN].bytecount = 0;         // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case READ_SW1:
            BD[EP0IN].address[0] = (uint8_t)sw_read(&sw1);
            BD[EP0IN].bytecount = 1;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        // case ENC_WRITE_REG:
        //     enc_writeReg(USB_setup.wValue, USB_setup.wIndex);
        //     BD[EP0IN].bytecount = 0;         // set EP0 IN byte count to 0
        //     BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
        //     break;
        case ENC_READ_REG:
            result = enc_readReg(USB_setup.wValue);
            BD[EP0IN].address[0] = result.b[0];
            BD[EP0IN].address[1] = result.b[1];
            BD[EP0IN].bytecount = 2;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case TOGGLE_LED3:
            led_toggle(&led3);
            BD[EP0IN].bytecount = 0;         // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case READ_SW2:
            BD[EP0IN].address[0] = (uint8_t)sw_read(&sw2);
            BD[EP0IN].bytecount = 1;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case READ_SW3:
            BD[EP0IN].address[0] = (uint8_t)sw_read(&sw3);
            BD[EP0IN].bytecount = 1;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case SEND_POS:
            BD[EP0IN].address[0] = position.b[0];
            BD[EP0IN].address[1] = position.b[1];
            BD[EP0IN].bytecount = 2;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case SEND_ANG:
            BD[EP0IN].address[0] = angle_now.b[0];
            BD[EP0IN].address[1] = angle_now.b[1];
            BD[EP0IN].bytecount = 2;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        default:
            USB_error_flags |= 0x01;    // set Request Error Flag
    }
}

void VendorRequestsIn(void) {
    switch (USB_request.setup.bRequest) {
        default:
            USB_error_flags |= 0x01;                    // set Request Error Flag
    }
}

void VendorRequestsOut(void) {
//    WORD32 address;
//
//    switch (USB_request.setup.bRequest) {
//        case ENC_WRITE_REGS:
//            enc_writeRegs(USB_request.setup.wValue.b[0], BD[EP0OUT].address, USB_request.setup.wLength.b[0]);
//            break;
//        default:
//            USB_error_flags |= 0x01;                    // set Request Error Flag
//    }
}
int16_t main(void) {
    init_clock();
    init_clock();
    init_ui();
    init_pin();
    init_spi();

    init_timer();
    init_oc();
    init_md();

    ENC_MISO = &D[1];
    ENC_MOSI = &D[0];
    ENC_SCK = &D[2];
    ENC_NCS = &D[3];

    read_angle.w=0x3FFF;
    angle_now.i=180;
    angle_prev.i=180;
    position.w=10;
    uint8_t loop = 0;
    // WORD angle;

    pin_digitalOut(ENC_NCS);
    pin_set(ENC_NCS);

    spi_open(&spi1, ENC_MISO, ENC_MOSI, ENC_SCK, 2e8,1);


    timer_setPeriod(&timer1, 0.05);
    timer_start(&timer1);

    InitUSB();                              // initialize the USB registers and serial interface engine
    while (USB_USWSTAT!=CONFIG_STATE) {     // while the peripheral is not configured...
        ServiceUSB();                       // ...service USB requests
    }

    // zeropt=angle_init(&loop);

    while (1) {
        ServiceUSB();
        if (timer_flag(&timer1)) {
            timer_lower(&timer1); 
            angle_prev=angle_now;                // service any pending USB requests
            angle_now = enc_readReg(read_angle);
            angle_now = mask_angle(angle_now);
            position=convert_Angle(angle_prev,angle_now,&loop);

            hardstop(position);
        }
        
    }
}
