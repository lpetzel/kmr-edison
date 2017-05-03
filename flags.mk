CC=gcc
CPP=g++
CFLAGS=-O2 -I/usr/include/modbus -I./include -I../include -pthread -std=c++11
PKGCONFIG=pkg-config
LFLAGS=-lmodbus -pthread -lprotobuf -L./lib -L../lib -lprotobuf_comm -lboost_system -lzbar $(shell $(PKGCONFIG) --libs opencv)
