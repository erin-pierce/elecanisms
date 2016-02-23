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
#define TOGGLE_LED3         3 
#define SEND_POS            4
#define SEND_ANG            5
#define SEND_DEF            6
#define SEND_MTRSPD         7
#define SEND_MTRDIR         8
#define SET_SCALE           9
#define SEND_DEFS           10

#define REG_MAG_ADDR        0x3FFE


WORD read_angle;
WORD angle_now;
WORD angle_prev;
WORD angle_prev_con;
WORD M1_cur;
WORD M2_cur;
WORD velocity;
WORD angle;  
WORD scale;
WORD direction;
WORD speed;
WORD deflection;
WORD negative;

_PIN *ENC_SCK, *ENC_MISO, *ENC_MOSI;
_PIN *ENC_NCS;

uint16_t setpoint = 13700;

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

WORD mask_angle(WORD raw_angle){
    WORD masked;
    masked.w=raw_angle.w & 0x3FFF;
    return masked;
}

WORD convert_Angle(WORD data_prev, WORD data_now, uint8_t *loop){
    if (data_prev.i-data_now.i > 10000){
        // led_on(&led3);
        *loop=*loop+1;

    }
    if (data_now.i-data_prev.i > 10000){
        // led_off(&led3);
        if (*loop !=0){*loop=*loop-1;}
    }


    data_now.w=data_now.w+(16384* *loop);

    return data_now;
}



WORD get_velocity(WORD angle_prev, WORD angle_now){
    WORD vel;
    vel.i=angle_prev.i-angle_now.i;
    return vel;
}

void switchdirection(){
    if (direction.w==1){
        direction.w = 0;
    }
    else{
        direction.w = 1;
    }
}

void nodes(WORD pos){
    WORD noderange; //distance between nodes
    // WORD absdef;

    if (pos.i > setpoint){
        deflection.w = pos.i - setpoint;
        negative.w = 1;
    }
    else{
        deflection.w = setpoint - pos.i;
        negative.w = 0; 
    }

    noderange.w = 5000;
    deflection.w = deflection.w%noderange.w;

    if (deflection.w > (noderange.w/2)){
        deflection.w = noderange.w - deflection.w;
        direction.w = 1;
    }
    else{
        direction.w = 0;

    }

    if (negative.w){
        switchdirection();
    }

    speed.w = deflection.w * scale.w;
    

    md_speed(&md2, speed.w);
    md_direction(&md2, direction.b[0]);

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
        case TOGGLE_LED3:
            led_toggle(&led3);
            BD[EP0IN].bytecount = 0;         // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case SEND_POS:
            BD[EP0IN].address[0] = angle_now.b[0];
            BD[EP0IN].address[1] = angle_now.b[1];
            BD[EP0IN].bytecount = 2;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case SEND_ANG:
            BD[EP0IN].address[0] = angle.b[0];
            BD[EP0IN].address[1] = angle.b[1];
            BD[EP0IN].bytecount = 2;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case SEND_DEF:
            BD[EP0IN].address[0] = deflection.b[0];
            BD[EP0IN].address[1] = deflection.b[1];
            BD[EP0IN].bytecount = 2;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case SEND_MTRSPD:
            BD[EP0IN].address[0] = speed.b[0];
            BD[EP0IN].address[1] = speed.b[1];
            BD[EP0IN].bytecount = 2;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case SEND_MTRDIR:
            BD[EP0IN].address[0] = direction.b[0];
            BD[EP0IN].bytecount = 1;         // set EP0 IN byte count to 1
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case SET_SCALE:
            scale=USB_setup.wValue;
            BD[EP0IN].bytecount = 0;         // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;         // send packet as DATA1, set UOWN bit
            break;
        case SEND_DEFS:
            BD[EP0IN].address[0] = negative.b[0];
            BD[EP0IN].bytecount = 1;         // set EP0 IN byte count to 1
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


    pin_analogIn(&A[0]);
    pin_analogIn(&A[1]);

    read_angle.w=0x3FFF;
    scale.i=1;
    direction.w=1;
    speed.w=0;
    angle_now.i=180;
    angle_prev.i=180;
    angle.w=10;
    uint8_t loop = 0;

    pin_digitalOut(ENC_NCS);
    pin_set(ENC_NCS);

    spi_open(&spi1, ENC_MISO, ENC_MOSI, ENC_SCK, 2e8,1);

    timer_setPeriod(&timer1, 0.05);
    timer_start(&timer1);

    InitUSB();  
                              // initialize the USB registers and serial interface engine
    while (USB_USWSTAT!=CONFIG_STATE) {     // while the peripheral is not configured...
        ServiceUSB();                       // ...service USB requests
    }  

    while(1){
        ServiceUSB();
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            angle_prev=angle_now; 
            angle_prev_con=angle;                // service any pending USB requests
            angle_now = enc_readReg(read_angle);
            angle_now = mask_angle(angle_now);
            angle=convert_Angle(angle_prev,angle_now,&loop);

            nodes(angle);
        }
    }    
} 