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

// get a pin parameter from cmd buffer
int getpin(char *buffer)
{
    int pin = atoi(buffer);
    return pin;
}
// get a value parameter from cmd buffer
int getvalue(char *buffer)
{
    int value = -1;
    buffer = strchr(buffer, '=');
    if(buffer)
    {
        buffer++;
        value = atoi(buffer);
    }
    return value;
}
// Format :dpin=val
static unsigned long handle_digital(char * buffer)
{
    int pin = getpin(buffer);
    int value = getvalue(buffer);
    if(value == -1)
        value = digitalRead(pin);
    else
        digitalWrite(pin,value);
    return value;
}

// Format :apin=val
static unsigned long handle_analog(char * buffer)
{
    int pin = getpin(buffer);
    int value = getvalue(buffer);
    if(value == -1)
        value = analogRead(pin);
    else
        analogWrite(pin, value);
    return value;
}

// Format :Ppin=val
static unsigned long handle_pin_mode(char * buffer)
{
    int pin = getpin(buffer);
    int value = getvalue(buffer);
    pinMode(pin, value);
    return value;
}

// Format :V0=ref
static unsigned long handle_reference_voltage(char * buffer)
{
    int pin = getpin(buffer);
    int value = getvalue(buffer);
    int ref = DEFAULT;
    switch (value)
    {
        case '1':
            ref = INTERNAL;
            break;
    }
    analogReference(ref);
    return ref;
}

// Format :E=[1|0]
static unsigned long handle_echo(char * buffer)
{
    int value = getvalue(buffer);
    if(value)
        do_echo = 1;
    else
        do_echo = 0;

    return (unsigned long)do_echo;
}

// Format :Rfunctio(params)
static unsigned long handle_run(char *buffer)
{
    int i;
    char *params = strchr(buffer, '(');
    if(params)
    {
        *params=0;
        params++;
    }
    for(i=0; extra_commands[i].name; i++)
    {
        if(strcmp(buffer, extra_commands[i].name)==0)
            return (extra_commands[i].func)(params);
    }
    return -1;
}

// Format :QNN
static unsigned long handle_query(char * buffer)
{
    unsigned long pin = getpin(buffer);
    return pin;
}

command builtin_commands[]={
    {'D',NULL, handle_digital},
    {'A',NULL, handle_analog},
    {'P',NULL, handle_pin_mode},
    {'V',NULL, handle_reference_voltage},
    {'E',NULL, handle_echo},
    {'R',NULL, handle_run},
    {'Q',NULL, handle_query},
    {0,NULL, NULL}
};

// Find the correct command to run
unsigned long handle_cmd(char *buffer)
{
    char chop = *buffer++;
    for(int cmd=0; builtin_commands[cmd].chop != 0; cmd++)
    {
        if(builtin_commands[cmd].chop == chop)
            return (builtin_commands[cmd].func)(buffer);
    }
    for(int cmd=0; extra_commands[cmd].chop != 0; cmd++)
    {
        if(extra_commands[cmd].chop == chop)
            return (extra_commands[cmd].func)(buffer);
    }
    return -1;
}
