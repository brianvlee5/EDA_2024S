all: Lab1

CXX = g++
LINKFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -g

Lab1: main.o parser.o 
	$(CXX) $(LINKFLAGS) main.o parser.o -o Lab1 
main.o: main.cpp
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" main.cpp -c  
parser.o: parser.cpp parser.h
	$(CXX) $(LINKFLAGS) parser.cpp -c

clean:
	rm -rf *.o *.gch Lab1






