CC = gcc
AR = ar

all: lib

lib: gg_serial.c gg_serial.h sys_win.c sys_win.h
	$(CC) -c -o gg_serial.o gg_serial.c
	$(CC) -c -o sys_win.o sys_win.c
	$(AR) -rcs libgg_serial.a gg_serial.o sys_win.o

clean:
	rm -f gg_serial.o
	rm -f sys_win.o
	rm -f libgg_serial.a
