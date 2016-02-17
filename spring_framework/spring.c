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
WORD M1_cur;
WORD M2_cur;

_PIN *ENC_SCK, *ENC_MISO, *ENC_MOSI;
_PIN *ENC_NCS;

uint8_t direction = 1;
uint16_t speed=0;
uint16_t position = 0;
uint16_t scalingParam = 267;

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

// void springBehavior(WORD angle, WORD cur){
//     uint16_t scaledSpeed = cur.w * scalingParam;
//     if (scaledSpeed < 0){
//         Motor.speed = scaledSpeed * -1;
//         Motor.direction = 0
//     }
//     else {
//         Motor.speed = scaledSpeed;
//         Motor.direction = 0
//     }
// }

void springBehavior(WORD angle, WORD cur){
    uint16_t Setpt = cur.w * scalingParam;
    if (scaledSpeed < 0){
        Motor.speed = scaledSpeed * -1;
        Motor.direction = 0
    }
    else {
        Motor.speed = scaledSpeed;
        Motor.direction = 0
    }
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

    while (1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);


            springBehavior(position)
            // direction = !direction;

            md_speed(&mdp, Motor.speed);
            md_direction(&mdp, Motor.direction);
        }
    }
}