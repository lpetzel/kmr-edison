include ../flags.mk

%.pb.cc: %.proto
	protoc --cpp_out=. $<

%.o: %.cc
	$(CPP) -c -o $@ $< $(CFLAGS)

all: MachineInstructions.pb.cc MachineDescription.pb.cc ProductColor.pb.cc Team.pb.cc MachineInstructions.pb.o MachineDescription.pb.o ProductColor.pb.o Team.pb.o

clean:
	rm *.o *.pb.cc *.pb.h
