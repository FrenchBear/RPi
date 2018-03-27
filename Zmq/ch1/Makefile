CC = gcc
CXX = g++
CFLAGS = -Wall -Wno-unused-function -std=gnu11
CXXFLAGS = -Wall
LDLIBS = -lm -lzmq

programs = 01version 02hwserver 02hwclient 03wuserver 03wuclient 04ventilator 04worker 04sink

make:	$(programs)

clean:
		rm -f $(programs)
