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

TARGETS=unittest ranger readtemp

CARGS=-g -O0 -Wall -I.. -DLIBARDIO_FUNCTS 
LOBJS=libardio.o

all: $(LOBJS) $(TARGETS)

unittest : unittest.c $(LOBJS)
	gcc $(CARGS) -o $@ $< $(LOBJS)

ranger : ranger.c $(LOBJS)
	gcc $(CARGS) -o $@ $< $(LOBJS)

readtemp : readtemp.c $(LOBJS)
	gcc $(CARGS) -o $@ $< $(LOBJS)

libardio.o : libardio.c ../libardio.h
	g++ $(CARGS) -c -o $@ $< 


clean:
	rm -f *.o
	rm -f $(TARGETS)

