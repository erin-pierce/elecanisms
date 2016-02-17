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

#define REG_MAG_ADDR        0x3FFE


WORD read_angle;
WORD angle_now;
WORD angle_prev;
WORD angle_prev_con;
WORD M1_cur;
WORD M2_cur;
WORD velocity;  

_PIN *ENC_SCK, *ENC_MISO, *ENC_MOSI;
_PIN *ENC_NCS;

uint8_t direction = 1;
uint8_t negative = 0;
uint16_t speed=0;
uint16_t position = 0;
uint16_t setpoint = 180;

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
    if (data_now.i-data_prev.i > 1000){
        led_on(&led3);
        *loop=1;
    }
    if (data_prev.i-data_now.i > 1000){
        led_off(&led3);
        *loop=0;
    }

    if (*loop==1){
        data_now.w=data_now.w+16384;
        led_on(&led2);
    }

    if (*loop==0){
        led_off(&led2);
    }
    return data_now;
}

WORD Current(){
    WORD cur;
    M1_cur=pin_read(&A[0]);
    M2_cur=pin_read(&A[1]);
    cur.w=M2_cur+M1_cur;
    return cur;
}

WORD get_velocity(WORD angle_prev, WORD angle_now){
    WORD vel;
    vel.i=angle_prev.i-angle_now.i;
    return vel;
}

void switchdirection(){
    if (direction==1){
        direction = 0;
    }
    else{
        direction = 1;
    }
}

void nodes(WORD pos){
    WORD deflection; //distance from node
    WORD noderange; //distance between nodes
    WORD scale;

    scale.i = 4000;

    if (pos.i > setpoint){
        deflection.i = pos.i - setpoint;
        negative = 1;
    }
    else{
        deflection.i = setpoint - pos.i;
        negative = 0; 
    }

    noderange.i = 10;
    deflection.i = deflection.i%noderange.i;

    if (deflection.i > (noderange.i/2)){
        deflection.i = noderange.i - deflection.i;
        direction = 1;
    }
    else{
        direction = 0;
    }

    if (negative){
        switchdirection()
    }

    speed = deflection.i * scale.i;
    

    md_speed(&md2, speed);
    md_direction(&md2, direction);

}

int16_t main(void) {
    init_clock();
    init_ui();          
    init_timer();
    init_pin();
    init_oc();
    init_md();

    led_on(&led2);
    led_on(&led3);

    timer_setPeriod(&timer1, 0.1);
    timer_start(&timer1);
    pin_analogIn(&A[0]);
    pin_analogIn(&A[1]);

    while(1){
        if (timer_flag(&timer1)) {
        timer_lower(&timer1);
        angle_prev=angle_now; 
        angle_prev_con=angle;                // service any pending USB requests
        angle_now = enc_readReg(read_angle);
        angle_now = mask_angle(angle_now);
        angle=convert_Angle(angle_prev,angle_now,&loop);

        nodes(angle)
        }
    }    
} 