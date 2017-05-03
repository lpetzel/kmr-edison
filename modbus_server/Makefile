CPP=g++
CFLAGS=-O2 -I /usr/include/modbus  -pthread
LFLAGS=-lmodbus

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp))

%.o: %.cpp
	$(CPP) -c -o $@ $< $(CFLAGS)

all: server_correct

server_correct: ServerCorrect.o
	$(CPP) -o $@ $^ $(LFLAGS)

clean:
	rm *.o

run: server_correct
	./server_correct 127.0.0.1 5000
