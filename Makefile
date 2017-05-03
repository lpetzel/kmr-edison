include flags.mk

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c)) $(patsubst %.cpp,%.o,$(wildcard *.cpp)) 

OBJ_BASE := BaseStation.o CapStation.o DeliveryStation.o Machine.o MachineProtoServer.o RingStation.o StorageStation.o TestModbus.o timeUtils.o protobuf/MachineInstructions.pb.o protobuf/MachineDescription.pb.o protobuf/ProductColor.pb.o protobuf/Team.pb.o

all: test_modbus proto_server
	cd modbus_server; make; cd ../proto_generator; make

protobuf/MachineInstructions.pb.o:
	cd protobuf; make

%.o: %.cpp
	$(CPP) -c -o $@ $< $(CFLAGS)

test_modbus: main_testModbus.o $(OBJ_BASE)
	$(CPP) -o $@ $^ $(LFLAGS)

proto_server: main_protoServer.o $(OBJ_BASE)
	$(CPP) -o $@ $^ $(LFLAGS)

clean:
	rm *.o

run_modbus: test_modbus
	LD_LIBRARY_PATH=lib ./test_modbus 127.0.0.1 5000

run_proto_server: proto_server
	./proto_server bs 127.0.0.1 5000 4444
