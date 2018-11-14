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

// Send a list of random raw commands to the arduino
void stream_test(int fd)
{
    char *test_commands[]={
        ":E=0\n",
        ":P13=1\n",
        ":D13=1\n",
        ":D0\n",
        ":D1\n",
        ":D13=0\n",
        ":A1\n",
        ":A2\n",
       ":E=1\n",
        ":Rmillis\n",
        ":Rmicros\n",
        ":V0=1\n",
        ":U0\n",
        ":M0\n",
        ":m0\n",
        NULL
    };

    size_t size;
    int cmdndx=0;
	char buffer[8192];
    time_t tstart = time(0);
	while(1)
	{
        int retry;
        size = writecmd(fd, test_commands[cmdndx]);
        for(size=0,retry=0; retry < 10 && size == 0; size = readresp(fd, buffer, sizeof(buffer))) ;
        if(size > 0)
            printf("%s\n", buffer);
        else
            printf("Command lost!\n");
        cmdndx++;
        if(test_commands[cmdndx]==NULL)
            cmdndx=0;

        if(time(0) - tstart > 5)
            break;
	}
}

// Blink the on-board LED using faux arduino functions
void blink_faux(int seconds)
{
    int i;
    pinMode(13,1);
    printf("blinking\n");
    
    for(i=0; i < seconds; i++)
    {
        digitalWrite(13, 0);
        usleep(500000);
        digitalWrite(13, 1);
        usleep(500000);
    }
}

// Blonk and report using ardio functions
void blink_ardio(int fd, int seconds)
{
    int i;
    for(i=0; i < seconds; i++)
    {
        printf("write digitalPin 13 = %d\n", ardioWrite(fd, 'D', 13, 1));
        printf("read digitalPin 13 = %d\n", ardioRead(fd, 'D', 13));
        usleep(500000);
        printf("write digitalPin 13 = %d\n", ardioWrite(fd, 'D', 13, 0));
        printf("read digitalPin 13 = %d\n", ardioRead(fd, 'D', 13));
        usleep(500000);
    }
    
}

int main(int argc, char **argv)
{
	char *device = "/dev/ttyACM0";
	int opt;

	while( (opt = getopt(argc, argv, "s:d:")) != -1)
	{
		switch(opt)
		{
			case 'd':
				device = optarg;
				printf("using device: %s\n", optarg);
				break;
		}
	}
	int fd = ardioOpen(device);

	if(fd < 0)
	{
		perror("Count open device");
		exit(-1);
	}

    blink_faux(5);
    stream_test(fd);
    blink_faux(5);
    blink_ardio(fd,5);

    ardioClose(fd);
}
