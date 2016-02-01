#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include <stdio.h>
#include "spi.h"



void as5048_init()
{
	float freq = 1000000;
	spi_open(&spi1,&D[1],&D[0],&D[2],freq);
}

void init_as5048(_AS5048 *self, _Pin *CS)
{
	self->ErrorFlag =0;
	self->RawAngle=0;
	self->CS=CS_pin;

	pin_digitalOut(self->CS);
    pin_clear(self->CS);	
}

void as5048_getRawAngle(_AS5048 *self)
{
	uint8_t MSB;
	uint8_t LSB;
	pin_set(self->CS);	
	MSB=spi_transfer(&spi1,0xFF);
	LSB=spi_transfer(&spi1,0xFF);
	pin_clear(self->CS);
	pin_set(self->CS);		
	MSB=spi_transfer(&spi1,0x00);
	LSB=spi_transfer(&spi1,0x00);
	pin_clear(self->CS);
	MSB=(uint16_t)MSB;
	MSB=MSB<<8;
	MSB|=(uint16_t)LSB;
	self->RawAngle=MSB;	
}