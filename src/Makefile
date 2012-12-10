BIN         = dope
CC          = gcc
SPEEDFLAGS  = -O3 -fomit-frame-pointer
DEBUGFLAGS  = -O0 -g -DDEBUG

LIBS        = -lgc
CCFLAGS     = $(LIBS) -Wall $(DEBUGFLAGS) #-DNO_OUTPUT
CFILES      = util.c file.c vm.c dope.c

all: dope 
dope: writer
	$(CC) $(CCFLAGS) -o $(BIN) $(CFILES)
writer:
	$(CC) -o writer $(LIBS) writer.c file.c util.c
clean:
	rm -rf *\.o *\~ $(BIN) writer $(BIN).dSYM
