CC=gcc
CPP=g++
CFLAGS=-O2 -I /usr/include/modbus  -pthread
LFLAGS=-lmodbus -pthread -lprotobuf -L ./lib -L . -lllsf_protobuf_comm

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp))

%.o: %.cpp
	$(CPP) -c -o $@ $< $(CFLAGS)

all: out

out: $(OBJECTS)
	$(CPP) -o $@ $< $(LFLAGS)
