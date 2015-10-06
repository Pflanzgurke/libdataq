all: lib_dataq.o
	clang -Weverything -o example example.c lib_dataq.o

lib_dataq.o: lib_dataq.h lib_dataq.c
	clang -Weverything -o lib_dataq.o -c lib_dataq.c

style:
	astyle --options=./astyle *.h *.c

check:
	cppcheck *.c *.h

doc:
	doxygen doxygen.conf

clean:
	rm *.o example *.orig
