CPP=g++
CFLAGS=-O2 -pthread
LFLAGS=-pthread -lprotobuf -L ../lib -lllsf_protobuf_comm -lboost_system

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp)) ../protobuf/proto.o

%.o: %.cpp
	$(CPP) -c -o $@ $< $(CFLAGS)


all: proto_gen

proto_gen: $(OBJECTS)
	$(CPP) -o $@ $^ $(LFLAGS)

clean:
	rm *.o

run: proto_gen
	LD_LIBRARY_PATH=../lib ./proto_gen 127.0.0.1 6000

