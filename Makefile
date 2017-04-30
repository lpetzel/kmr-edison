include flags.mk

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp)) protobuf/proto.o

%.o: %.cpp
	$(CPP) -c -o $@ $< $(CFLAGS)

all: out

out: $(OBJECTS)
	$(CPP) -o $@ $^ $(LFLAGS)

clean:
	rm *.o

run: out
	LD_LIBRARY_PATH=lib ./out 127.0.0.1
