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

#ifndef _LIBARDIO_H_
#define _LIBARDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define CMDBUF_SIZE 16
#define SELECT_USEC 10000
#define SERIAL_BAUD 38400
#define READ_RETRY  5

// You must define ' LIBARDIO_FUNCTS' to get function declarations.
// They break arduino builds.
#ifdef LIBARDIO_FUNCTS
extern int  writecmd(int fd, char *buffer);
extern int  readresp(int fd, char *buffer, int max);

extern int  ardioOpen(char *device);
extern int  ardioRead (int fd, char cmd, int pin);
extern int  ardioWrite (int fd, char cmd, int pin, int value);
extern void ardioClose(int fd);

extern int  digitalRead(int pin);
extern int  digitalWrite(int pin, int value);
extern int  analogRead(int pin);
extern int  analogWrite(int pin, int value);
extern int  pinMode(int pin, int mode);
extern int  analogReference(int type);
#endif

#ifdef __cplusplus
}
#endif

#endif // _LIBARDIO_H_
