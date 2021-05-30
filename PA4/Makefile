all: PA4_106502013.out

#declare variables
CC = g++ -std=c++11
INSTDIR = /usr/local/bin
INCLUDE = .
CFLAFS = -g -Wall -ansi
LIBS += -framework CoreFoundation
.PHONY: run

PA4_106502013.out: PA4_106502013.cpp floorplan.cpp block.cpp treeNode.cpp 
	$(CC)  -o PA4_106502013.out PA4_106502013.cpp floorplan.cpp block.cpp treeNode.cpp
clean:
	rm -f PA4_106502013.out PA4_106502013.o
run: 
	./PA4_106502013.out $(testcase) $(output)