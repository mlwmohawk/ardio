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
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termio.h>
#include <fcntl.h>
#include <time.h>
#include <sys/select.h>
#include <string.h>
#include <math.h>
#include <getopt.h>
#include <errno.h>

#include "libardio.h"

extern int last_fd;

// Emulate Arduino calls with last_fd

int digitalRead (int pin)
{
    return ardioRead (last_fd, 'D', pin);
}

int digitalWrite (int pin, int value)
{
    return ardioWrite (last_fd, 'D', pin, value);
}

int analogRead (int pin)
{
    return ardioRead (last_fd, 'A', pin);
}

int analogWrite (int pin, int value)
{
    return ardioWrite (last_fd, 'A', pin, value);
}

int pinMode (int pin, int mode)
{
    return ardioWrite (last_fd, 'P', pin, mode);
}

int analogReference (int type)
{
    return ardioWrite (last_fd, 'V', 0, type);
}



