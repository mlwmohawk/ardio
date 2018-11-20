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

char do_echo = 1;
static char buffer[CMDBUF_SIZE];

// Read and optionally echo a charecter from the serial port
int ardio_serial::readchar(void)
{
    int c = Serial.read();
    if(c>=0 && c != '\n' && c!= '\r' && do_echo == 1)
        Serial.write((char)c);
    return c;
}
void ardio_serial::setup()
{
    Serial.begin(SERIAL_BAUD,SERIAL_8N1);
    Serial.print("<ready ");
    Serial.print(ARDIO_MAJOR);
    Serial.print(".");
    Serial.print(ARDIO_MINOR);
    Serial.print(">\n");
}

void ardio_serial::loop(int type)
{
    if(Serial.available())
    {
        int  ch = readchar();
        if ( ch == ':' )
        {
            int i;
            for(i=0; i < CMDBUF_SIZE;)
            {
                while(!Serial.available())
                    yield();
                ch = readchar();
                if(ch == -1 || ch == '\n')
                    break;
                if(ch == '\r') // Ignore "carriage return"
                    continue;

                buffer[i]=(char)ch;
                i++;
            }
            buffer[i]=0;
            unsigned long value = handle_cmd(buffer);
            Serial.print('<');
            Serial.print(value);
            Serial.print(">\n");
        }
    }
}

