#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "oc.h"
#include "pin.h"
#include "timer.h"
#include "md.h"

uint8_t direction = 1;
uint8_t up=1;
uint16_t speed=12000;
int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_oc();
    init_md();

    led_on(&led2);
    led_on(&led3);

    timer_setPeriod(&timer1, 0.5);
    timer_start(&timer1);

    while (1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);

            if (speed > 24000){
                up=0;
            }

            if (speed < 12000){
                up=1;
            }

            if (up){
                speed=speed+300;
            }
            if (!up){
                speed=speed-300;
            }

            // direction = !direction;

            md_speed(&mdp, speed);
            md_direction(&mdp, direction);
        }
    }
}
