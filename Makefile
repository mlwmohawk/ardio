#    Copyright (C) 2018 Mark L. Woodward
# 
#    This library is free software; you can redistribute it and/or
#    modify it under the terms of the GNU Library General Public
#    License as published by the Free Software Foundation; either
#    version 2 of the License, or (at your option) any later version.
# 
#    This library is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#    Library General Public License for more details.
# 
#    You should have received a copy of the GNU Library General Public
#    License along with this library; if not, write to the Free
#    Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
#    MA 02111-1307, USA
# 
#    If you want support or to commercially license this library, the author
#    can be reached at markw@mohawksoft.com

ifndef ARDUINO_VER
ARDUINO_VER=1.8.7
endif
DEVICE =  /dev/ttyACM0
UPLOAD_RATE = 115200 
AVRDUDE_PROGRAMMER = arduino
#MCU = atmega328
MCU = atmega328p
PMCU=ATMEGA328P
F_CPU = 16000000
PROJECT=ardio
C_SRC=
CXX_SRC=ardio.cpp ardtask.cpp ardserial.cpp ultrasonic.cpp utils.cpp config.cpp
SUBDIRS = examples
EXTRA_TARGETS = subdirs
EXTRA_SRC = 

include makefiles/Arduino.mak

install:
	echo "install not supported"

clean_extra:
	rm -f *.o
	rm -f $(EXTRA_TARGETS)
	for dir in $(SUBDIRS); do make -C $$dir clean; done

subdirs : $(SUBDIRS)
	for dir in $(SUBDIRS); do make -C $$dir; done
