CC=gcc
CPP=g++
CFLAGS=-O2 -I /usr/include/modbus -pthread
LFLAGS=-lmodbus -pthread

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp))

%.o: %.cpp
	$(CPP) -c -o $@ $< $(CFLAGS)

all: $(OBJECTS)
	$(CPP -o out $< $(LFLAGS)
