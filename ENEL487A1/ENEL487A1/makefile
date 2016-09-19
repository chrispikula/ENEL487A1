# The compiler we will use will be g++
CC=g++
# The flags we will pass to our compiler
CFLAGS=-c -Wall

all: complex

complex: complex.o
	$(CC) complex.o -o complex

complex.o: complex.cpp
	$(CC) $(CFLAGS) complex.cpp

clean: rm *o complex
