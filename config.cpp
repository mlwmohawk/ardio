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
#include "ultrasonic.h"

#define ENABLE_ULTRA

ardio_serial serial;

#ifdef ENABLE_ULTRA
UltrasonicSensor us0(20);
UltrasonicSensor us1(21);

unsigned long readultra(char *buffer)
{
    if(*buffer == '0')
        return us0.getvalue();
    else
        return us1.getvalue();
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
    {'M',"millis",readmillis},
    {'m',"micros",readmicros},
#ifdef ENABLE_ULTRA
    {'U',"ultrasonic",readultra},
#endif
    {0,NULL,NULL}
};

ardio_task *tasks[]={
    &serial,
#ifdef ENABLE_ULTRA
    &us0,
    &us1,
#endif
    NULL
};

