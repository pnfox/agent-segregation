SHELL = /bin/sh
CXX = g++
CFLAGS = -Wall -std=c++17
SFML = $(shell pkg-config sfml-graphics --libs)

all: population.o
	$(CXX) $(CFLAGS) $(SFML) -o final main.cpp population.o

population.o:
	$(CXX) $(CFLAGS) $(SFML) -c population.cpp

clean:
	rm final *.o
