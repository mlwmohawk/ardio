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


ARDUINO_BUILD=184
ARDUINO = $(INSTALL_DIR)/hardware/arduino/avr/cores/arduino
VARIANTS = $(INSTALL_DIR)/hardware/arduino/avr/variants/standard
ARDUINO_LIB = $(INSTALL_DIR)/hardware/arduino/avr/libraries
AVRDUDE_PATH = $(INSTALL_DIR)/hardware/tools/avr/bin
AVRDUDE_CONF = $(INSTALL_DIR)/hardware/tools/avr/etc/avrdude.conf
AVRDUDE = $(AVRDUDE_PATH)/avrdude

C_MODULES =  \
srcdir/hooks.c \
srcdir/WInterrupts.c \
srcdir/wiring_analog.c \
srcdir/wiring.c \
srcdir/wiring_digital.c \
srcdir/wiring_pulse.c \
srcdir/wiring_shift.c

CXX_MODULES = \
srcdir/abi.cpp \
srcdir/CDC.cpp \
srcdir/HardwareSerial0.cpp \
srcdir/HardwareSerial1.cpp \
srcdir/HardwareSerial2.cpp \
srcdir/HardwareSerial3.cpp \
srcdir/HardwareSerial.cpp \
srcdir/IPAddress.cpp \
srcdir/main.cpp \
srcdir/new.cpp \
srcdir/PluggableUSB.cpp \
srcdir/Print.cpp \
srcdir/Stream.cpp \
srcdir/Tone.cpp \
srcdir/USBCore.cpp \
srcdir/WMath.cpp \
srcdir/WString.cpp \

