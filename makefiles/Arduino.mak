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

#
# Basic directory structure
#
INSTALL_DIR = /opt/arduino/arduino-$(ARDUINO_VER)
include makefiles/Arduino-$(ARDUINO_VER).mak    

#
# Substitute module sources and dependencies
#
OBJDIR = image

SRC_T = $(C_MODULES) $(C_SRC)
CXXSRC_T = $(CXX_MODULES) $(CXX_SRC)

SRC = $(subst srcdir, $(ARDUINO), $(SRC_T) )
CXXSRC = $(subst srcdir, $(ARDUINO), $(CXXSRC_T) )

OBJ_T = $(C_MODULES:.c=.o) $(C_SRC:.c=.o) $(CXX_MODULES:.cpp=.o) $(CXX_SRC:.cpp=.o) $(ASRC:.S=.o)
OBJ_MODULES_T = $(C_MODULES:.c=.o) $(C_SRC:.c=.o) $(CXX_MODULES:.cpp=.o) $(CXX_SRC:.cpp=.o)

OBJ = $(subst srcdir, $(OBJDIR), $(OBJ_T))
OBJ_MODULES = $(subst srcdir, $(OBJDIR), $(OBJ_MODULES_T))

DEPS = $(SRC:.c=.d) $(CXXSRC:.cpp=.d) $(ASRC:.S=.d)
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)


# Build flags
FORMAT = ihex
CDEFS = -DF_CPU=$(F_CPU)L -DARDUINO=$(ARDUINO_BUILD)
CINCS = -I$(ARDUINO)  -I$(VARIANTS) -I$(ARDUINO_LIB) $(CINCS_PROJ)
COPT = -Os
CDEBUG = -g$(DEBUG)
CWARN = -Wall
CTUNING = -ffunction-sections -fdata-sections
CXXTUNING = -fno-exceptions -ffunction-sections -fdata-sections
CFLAGS = $(CDEBUG) $(COPT) $(CWARN) $(CTUNING) $(CDEFS) $(CINCS) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CDEBUG) $(COPT) $(CWARN) $(CXXTUNING) $(CDEFS) $(CINCS)
LDFLAGS = $(COPT) -lm -Wl,--gc-sections
ALL_CFLAGS = $(CFLAGS) -mmcu=$(MCU)
ALL_CXXFLAGS = $(CXXFLAGS) -mmcu=$(MCU)
ALL_ASFLAGS = -x assembler-with-cpp $(ASFLAGS) -mmcu=$(MCU)
ALL_LDFLAGS = $(LDFLAGS) -mmcu=$(MCU)

# Build programs
AVR_TOOLS_PATH = /usr/bin
CC = $(AVR_TOOLS_PATH)/avr-gcc
CXX = $(AVR_TOOLS_PATH)/avr-g++
LD = $(AVR_TOOLS_PATH)/avr-gcc
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR  = $(AVR_TOOLS_PATH)/avr-ar
SIZE = $(AVR_TOOLS_PATH)/avr-size
NM = $(AVR_TOOLS_PATH)/avr-nm

# Allow use of system install of avrdude
ifdef SYSTEM_AVRDUDE
    AVRDUDE=$(SYSTEM_AVRDUDE)
    AVRDUDE_CONF=$(SYSTEM_AVRDUDE_CONF)
endif

AVRDUDE_DEVICE = $(DEVICE)
AVRDUDE_WRITE_FLASH = -U flash:w:$(OBJDIR)/$(PROJECT).hex
AVRDUDE_FLAGS = -V -F -C $(AVRDUDE_CONF) \
-p $(PMCU) -P $(AVRDUDE_DEVICE) -c $(AVRDUDE_PROGRAMMER) \
-b $(UPLOAD_RATE) -D

all: build $(EXTRA_TARGETS) subdirs

build: $(OBJDIR) elf hex

$(OBJDIR) :
	mkdir -p $(OBJDIR)

$(OBJDIR)/$(PROJECT): 
	test -d $(OBJDIR) || mkdir $(OBJDIR)

elf: $(OBJDIR)/$(PROJECT).elf
hex: $(OBJDIR)/$(PROJECT).hex
eep: $(OBJDIR)/$(PROJECT).eep
lss: $(OBJDIR)/$(PROJECT).lss
sym: $(OBJDIR)/$(PROJECT).sym

# Program the device.
upload: $(OBJDIR)/$(PROJECT).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	$(OBJCOPY) -O $(FORMAT) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
    --no-change-warnings --change-section-lma .eeprom=0 $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@

$(OBJDIR)/$(PROJECT).elf: $(OBJDIR)/core.a
	$(LD) $(ALL_LDFLAGS) -o $@ $(OBJDIR)/core.a

$(OBJDIR)/core.a: $(OBJ_MODULES)
	@for i in $(OBJ_MODULES); do echo $(AR) rcs $(OBJDIR)/core.a $$i; $(AR) rcs $(OBJDIR)/core.a $$i; done

depend : $(DEPS)

$(OBJDIR)/%.o : $(ARDUINO)/%.c
	$(CC) -c $(ALL_CFLAGS) $< -o $@

$(OBJDIR)/%.o : $(ARDUINO)/%.cpp
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@

.cpp.o:
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@

.c.o:
	$(CC) -c $(ALL_CFLAGS) $< -o $@

clean: clean_extra
	rm -f  $(OBJDIR)/$(PROJECT).hex $(OBJDIR)/$(PROJECT).eep $(OBJDIR)/$(PROJECT).cof $(OBJDIR)/$(PROJECT).elf \
	$(OBJDIR)/$(PROJECT).map $(OBJDIR)/$(PROJECT).sym $(OBJDIR)/$(PROJECT).lss $(OBJDIR)/core.a \
	$(OBJ) $(OBJ_MODULES)

allclean:
	rm -f  $(OBJDIR)/$(PROJECT).hex $(OBJDIR)/$(PROJECT).eep $(OBJDIR)/$(PROJECT).cof $(OBJDIR)/$(PROJECT).elf \
    $(OBJDIR)/$(PROJECT).map $(OBJDIR)/$(PROJECT).sym $(OBJDIR)/$(PROJECT).lss $(OBJDIR)/core.a \
    $(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d)

.PHONY: all build elf hex eep lss sym program coff extcoff clean clean_extra subdirs
