include flags.mk

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp)) protobuf/proto.o

OBJ_BASE := BaseStation.o CapStation.o DeliveryStation.o Machine.o MachineProtoServer.o RingStation.o StorageStation.o TestModbus.o timeUtils.o protobuf/proto.o

%.o: %.cpp
	$(CPP) -c -o $@ $< $(CFLAGS)


test_modbus: main_testModbus.o $(OBJ_BASE)
	$(CPP) -o $@ $^ $(LFLAGS)

proto_server: main_protoServer.o $(OBJ_BASE)
	$(CPP) -o $@ $^ $(LFLAGS)

all: test_modbus proto_server

clean:
	rm *.o

run_modbus: test_modbus
	LD_LIBRARY_PATH=lib ./test_modbus 127.0.0.1 5000

run_proto_server: proto_server
	LD_LIBRARY_PATH=lib ./proto_server bs 127.0.0.1 5000 6000
