all: QM.out

#declare variables
CC = g++ -std=c++11
INSTDIR = /usr/local/bin
INCLUDE = .
CFLAFS = -g -Wall -ansi
LIBS += -framework CoreFoundation

QM.out: QM.cpp Quine_McCluskey.cpp
	$(CC) -o QM.out main.cpp Quine_McCluskey.cpp

clean:
	rm -f QM.out QM.o Quine_McCluskey.o
