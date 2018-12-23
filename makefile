#makefile for P4

all: *.cpp
	g++ -std=c++11 -g -o comp  *.cpp

clean:
	$(RM) comp *.asm
