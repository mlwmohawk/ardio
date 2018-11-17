/*
    Copyright (C) 2018 Mark L. Woodward
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
    MA 02111-1307, USA
 
    If you want support or to commercially license this library, the author
    can be reached at markw@mohawksoft.com
*/ 
#include <Arduino.h>
#include "libardio.h"
#include "ardio.h"

/*
 * This module is where you construct your image. Think of ardio as a toolkit for
 * building your project. As a baseline, it functions like a simple I/O board
 * but you can add functionality as either tasks or functions or a combination
 * of both.
 * Define "ENABLE_ULTRA" will expose the ultrasonic ranging code as a ":U" function.
 */

#define ENABLE_ULTRA
#define ENABLE_DS18B20

ardio_serial serial;

#ifdef ENABLE_ULTRA
#include "ultrasonic.h"
UltrasonicSensor us0(7);
unsigned long readultra(char *buffer)
{
    return us0.getvalue();
}
#endif

#ifdef ENABLE_DS18B20
#include "DS18B20.h"
DS18B20 ds18b20;

unsigned long readtemp(char *buffer)
{
	return ds18b20.getvalue(0);
}

#endif

unsigned long readmillis(char *buffer)
{
    return millis();
}
unsigned long readmicros(char *buffer)
{
    return micros();
}

command extra_commands[]={
    {'M',"millis",readmillis},  // Example of adding your own command.
    {'m',"micros",readmicros},
#ifdef ENABLE_ULTRA
    {'U',"ultrasonic",readultra},
#endif
#ifdef ENABLE_DS18B20
	{'T',"readtemp",readtemp},
#endif
    {0,NULL,NULL}
};

ardio_task *tasks[]={
    &serial,
#ifdef ENABLE_ULTRA
    &us0,
#endif
    NULL
};

