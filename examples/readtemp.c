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
        perror("Can not open device");
        exit(-1);
    }

	while( 1 )
	{
		double temp = (double) ardioRead(fd, 'T', 0);
		temp = temp * 0.0078125; // Convert from raw to C

		double f = ((temp*9)/5) + 32;

        printf("%0.1f (%0.1f)\n", temp, f);
		usleep(100000);
	}

    ardioClose(fd);
}
