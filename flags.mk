CC=gcc
CPP=g++
CFLAGS=-O2 -I /usr/include/modbus  -pthread
LFLAGS=-lmodbus -pthread -lprotobuf -L ./lib -lllsf_protobuf_comm -lboost_system
