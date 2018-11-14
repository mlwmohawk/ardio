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

int ardio_debug=0;

// Ugly static variable, last opened arduino
static int last_fd = -1;

// Configure serial properties on a file descriptor
static void setparms (int f, int newbaud, char *par, char *bits)
{
	int spd = -1;
	int bit = bits[0];
	struct termios tty;

	tcgetattr (f, &tty);

	// We generate mark and space parity ourself. 
	if (bit == '7' && (par[0] == 'M' || par[0] == 'S'))
		bit = '8';

	switch (newbaud)
	{
	case 0:
		spd = 0;
		break;
	case 300:
		spd = B300;
		break;
	case 600:
		spd = B600;
		break;
	case 1200:
		spd = B1200;
		break;
	case 2400:
		spd = B2400;
		break;
	case 4800:
		spd = B4800;
		break;
	case 9600:
		spd = B9600;
		break;
	case 19200:
		spd = B19200;
		break;
	case 38400:
		spd = B38400;
		break;
	case 57600:
		spd = B57600;
		break;
	case 115200:
		spd = B115200;
		break;
	}

	if (spd != -1)
	{
		cfsetospeed (&tty, (speed_t) spd);
		cfsetispeed (&tty, (speed_t) spd);
	}

	switch (bit)
	{
	case '5':
		tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS5;
		break;
	case '6':
		tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS6;
		break;
	case '7':
		tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS7;
		break;
	case '8':
	default:
		tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
		break;
	}
	cfmakeraw (&tty);
	tcsetattr (f, TCSANOW, &tty);
}

// Uses select to wait for space in write buffer
static int waitwrite (int fd, int usec)
{
	struct timeval tv;
	fd_set fdset;
	tv.tv_usec = SELECT_USEC;
	tv.tv_sec = 0;
	FD_ZERO (&fdset);
	FD_SET (fd, &fdset);
	return select (1, NULL, &fdset, NULL, &tv);
}

// Uses select to wait for something to arrive in read buffer
static int waitread (int fd, int usec)
{
	struct timeval tv;
	fd_set fdset;
	tv.tv_usec = SELECT_USEC;
	tv.tv_sec = 0;
	FD_ZERO (&fdset);
	FD_SET (fd, &fdset);
	return select (1, &fdset, NULL, NULL, &tv);
}

// Writes a command string to USB
// File descriptor is non-blocking, so "-1"
// may not really be an error, just need to
// wait on USB
int writecmd (int fd, char *buffer)
{
	int count = 0;
    char *p = buffer;
	while (*buffer)
	{
		ssize_t size = write (fd, buffer, 1);
		if (size == -1)
		{
			if (errno != EAGAIN)
			{
				perror ("Error reading device\n");
				exit (0);
			}
			waitwrite (fd, SELECT_USEC);
		}
		else if (size > 0)
		{
			buffer++;
			count++;
		}
	}
	fdatasync (fd);
    if(ardio_debug > 1)
        printf("{{%s}}\n", p);
    usleep(1000);
	return count;
}

// Read the latest response from USB
// File descriptor is non-blocking so "-1"
// may just mean that we need to wait for the
// USB device to finish writing
int readresp (int fd, char *buffer, int max)
{
	char c;
	int count = 0;
	int retries = 0;
    char *p = buffer;
	memset (buffer, 0, max);
	while (1)
	{
		c = 0;
		ssize_t size = read (fd, &c, 1);
		if (size >= 0)
		{
			retries = 0;
			if (c == 0 || c == '\r')
            {
                if(ardio_debug > 0)
                    printf("c is invalid: %x\n", c);
				continue;
            }
			if (c == '\n')
			{
				*buffer++ = 0;
				break;
			}
			*buffer++ = c;
			count++;
		}
		else
		{
			if (errno != EAGAIN)
			{
				perror ("Error reading device\n");
				exit (0);
			}
			waitread (fd, SELECT_USEC);
		}
		if (count >= max)
			break;
		if (retries++ >= READ_RETRY )
        {
            if(ardio_debug > 0)
                printf("exceeded retry count\n");
			break;
        }
	}
    if(ardio_debug > 1)
    {
        printf("count=%d\n", count);
        printf("[[%s]]\n", p);
    }
	return count;
}

// Open a USB/Arduino. 
// Wait for it to reset, then synchronize communication
int ardioOpen (char *device)
{
	int fd = open (device, O_RDWR);

	if (fd < 0)
	{
		perror ("Count open device");
		exit (-1);
	}

	setparms (fd, SERIAL_BAUD, "N", "8"); // Set baud
	usleep (250000);	// Give arduino time to reset after connection

    // Set to non-blocking
	unsigned int flags = fcntl (fd, F_GETFL);
	fcntl (fd, F_SETFL, flags | O_NONBLOCK);

	last_fd = fd;

    // get IPC working
    // This flushes anything left in the USB buffer and gives
    // Arduino a chance to sync-up
    char buffer[256];

    readresp(fd, buffer,sizeof(buffer));
    if(ardio_debug > 0)
        printf("[[%s]]\n", buffer);
    
    strcpy(buffer, ":Q42\n");
    writecmd(fd, buffer);
    usleep(1000);

    // Wake arduino up by sending it query ops
    for(int i=0; 1; i++)
    {
        memset(buffer,0,sizeof(buffer));
        if(readresp(fd,buffer, sizeof(buffer))==0)
        {
            strcpy(buffer, ":Q42\n");
            writecmd(fd, buffer);
            usleep(1000);
        }
        else
        {
            // Got one back, this means arduino is ready
            if(ardio_debug > 0)
                printf("[[%s]]\n", buffer);
            if(strstr(buffer, "<42>"))
                break;
        }
    }
    // flush out any remaining I/O
    for(int i=0; 1; i++)
    {
        memset(buffer,0,sizeof(buffer));
        if(readresp(fd,buffer, sizeof(buffer))==0)
            break;
        if(ardio_debug > 0)
            printf("[[%s]]\n", buffer);
    }
	return fd;
}

// Here for completeness, but in the future they may be good reason to
// send one last command before disconnecting.
void ardioClose(int fd)
{
    close(fd);

    if(last_fd == fd)
        last_fd = -1;
}

// Execute a "read" operation
int ardioRead (int fd, char cmd, int pin)
{
	char cmdbuf[CMDBUF_SIZE];
	snprintf (cmdbuf, sizeof (cmdbuf), ":%c%d\n", cmd, pin);
	writecmd (fd, cmdbuf);
	memset (cmdbuf, 0, CMDBUF_SIZE);
	readresp (fd, cmdbuf, CMDBUF_SIZE);
	char *resp = strchr (cmdbuf, '<');
	if (!resp)
		return -1;
	else
		return atoi (&resp[1]);
}

// Execute a "write" operation
int ardioWrite (int fd, char cmd, int pin, int value)
{
	char cmdbuf[CMDBUF_SIZE];
	snprintf (cmdbuf, sizeof (cmdbuf), ":%c%d=%d\n", cmd, pin, value);
	writecmd (fd, cmdbuf);
	memset (cmdbuf, 0, CMDBUF_SIZE);
	readresp (fd, cmdbuf, CMDBUF_SIZE);
	char *resp = strchr (cmdbuf, '<');
	if (!resp)
		return -1;
	else
		return atoi (&resp[1]);
}

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
