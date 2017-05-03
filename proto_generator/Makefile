CPP=g++
CFLAGS=-O2 -pthread -I../include -std=c++11
LFLAGS=-pthread -lprotobuf -L ../lib -lprotobuf_comm -lboost_system

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp)) ../protobuf/MachineInstructions.pb.o ../protobuf/MachineDescription.pb.o ../protobuf/ProductColor.pb.o ../protobuf/Team.pb.o

%.o: %.cpp
	$(CPP) -c -o $@ $< $(CFLAGS)


all: proto_gen

proto_gen: $(OBJECTS)
	$(CPP) -o $@ $^ $(LFLAGS)

clean:
	rm *.o

run: proto_gen
	./proto_gen 127.0.0.1 4444

