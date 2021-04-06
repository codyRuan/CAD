all: PA2_106502013.out

#declare variables
CC = g++
INSTDIR = /usr/local/bin
INCLUDE = .
CFLAFS = -g -Wall -ansi
LIBS += -framework CoreFoundation

PA2_106502013.out: PA2_106502013.o graph.o
	$(CC) -o PA2_106502013.out PA2_106502013.o graph.o
PA2_106502013.o: PA2_106502013.cpp
	$(CC) -I$(INCLUDE) $(CFLAGS) -c PA2_106502013.cpp
graph.o: graph.cpp graph.h
	$(CC) -I$(INCLUDE) $(CFLAGS) -c graph.cpp

clean:
	rm -f PA2_106502013.out PA2_106502013.o graph.o
