all: Lab3

CXX = g++
LINKFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -g -std=c++11

Lab3: main.o parser.o router.o coordinate.o
	$(CXX) $(LINKFLAGS) main.o parser.o router.o coordinate.o -o Lab3 
main.o: main.cpp
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" main.cpp -c  
parser.o: parser.cpp parser.h
	$(CXX) $(LINKFLAGS) parser.cpp -c  
router.o: router.cpp router.h
	$(CXX) $(LINKFLAGS) router.cpp -c  
coordinate.o: coordinate.cpp coordinate.h
	$(CXX) $(LINKFLAGS) coordinate.cpp -c  

test:
	./Lab3 ./case1/test.in

test2:
	./Lab3 ./case2/case2.in

runall:
	./Lab3 case1.in case1.out
	./Lab3 case2.in case2.out
	./Lab3 case3.in case3.out
	./Lab3 case4.in case4.out
	./Lab3 case5.in case5.out

clean:
	rm -rf *.o *.gch Lab3






