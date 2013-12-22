# Makefile for predictGMAP

#G++FLAG = -g -Wall -Werror
G++FLAG = -O3 -m64 -mmmx -msse -msse2

all : predictGMAP

predictGMAP : predictGMAP.o param_t.o mapdata_t.o
	g++ -o predictGMAP predictGMAP.o param_t.o mapdata_t.o

predictGMAP.o : predictGMAP.cpp
	g++ $(G++FLAG) -c predictGMAP.cpp 

mapdata_t.o : mapdata_t.cpp
	g++ $(G++FLAG) -c mapdata_t.cpp

param_t.o : param_t.cpp
	g++ $(G++FLAG) -c param_t.cpp

clean :
	rm *.o
