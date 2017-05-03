CC=gcc
CPP=g++
CFLAGS=-O2 -I/usr/include/modbus  -pthread -std=c++11 -I/usr/local/include/protobuf_comm 
LFLAGS=-lmodbus -pthread -lprotobuf -L ./lib -lprotobuf_comm -lboost_system
