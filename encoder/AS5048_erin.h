#ifndef _AS5048_H_
#define _AS5048_H_

#include <stdint.h>
#include "pin.h"

void as5048_init(void);

typedef struct{
	int16_t RawAngle;
	int ErrorFlag;
	_PIN *CS;
} _AS5048;

extern _AS5048;

void init_as5048(_AS5048 *, _PIN *);

void as5048_getRawAngle(_AS5048 *);

#endif

