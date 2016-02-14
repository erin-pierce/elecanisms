#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "oc.h"
#include "pin.h"
#include "timer.h"
#include "md.h"

uint8_t direction = 1;
uint16_t speed=0;
uint16_t position = 0;
uint16_t scalingParam = 267;

struct Motor{
    uint16_t speed;
    uint8_t direction;   
};

void springBehavior(deflection){
    uint16_t scaledSpeed = deflection * scalingParam;
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
