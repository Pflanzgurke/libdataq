all: lib_dataq.o
	gcc -o example example.c lib_dataq.o

lib_dataq.o: lib_dataq.h lib_dataq.c
	gcc -o lib_dataq.o -c lib_dataq.c

