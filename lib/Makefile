

C_SRC= ardiocomm.c ardiofaux.c libds18b20.c
OBJ = $(C_MODULES:.c=.o) $(C_SRC:.c=.o) $(CXX_MODULES:.cpp=.o) $(CXX_SRC:.cpp=.o) $(ASRC:.S=.o)

CARGS=-g -O0 -fPIC -Wall -I.. -DLIBARDIO_FUNCTS 

ALL=libardio.a libardio.so

all : $(ALL)


libardio.a : $(OBJ)
	ar rcs $@ $(OBJ)	

libardio.so : $(OBJ)
	gcc -shared -o $@ $(OBJ) 

clean:
	rm -f $(ALL)
	rm -f $(OBJ)

.c.o:
	gcc -c $(CARGS) -o $@ $<

