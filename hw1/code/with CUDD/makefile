all: Lab1

CXX = g++
LINKFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -g

Lab1: main.o parser.o bdd_manager.o
	$(CXX) $(LINKFLAGS) main.o parser.o bdd_manager.o -o Lab1 ./cudd-3.0.0/cudd/.libs/libcudd.a
main.o: main.cpp
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" main.cpp -c  
parser.o: parser.cpp parser.h
	$(CXX) $(LINKFLAGS) parser.cpp -c
bdd_manager.o: bdd_manager.cpp bdd_manager.h
	$(CXX) $(LINKFLAGS) bdd_manager.cpp -c -I ./cudd-3.0.0/cudd -I ./cudd-3.0.0/util -I ./cudd-3.0.0
	#-c to don't need to compile with .a

clean:
	rm -rf *.o *.gch Lab1






