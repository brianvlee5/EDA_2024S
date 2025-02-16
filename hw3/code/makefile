all: lab3

CXX = g++
LINKFLAGS = -pedantic -Wall -fomit-frame-pointer -funroll-all-loops -O3 -g -std=c++11

lab3: main.o parser.o router.o coordinate.o
	$(CXX) $(LINKFLAGS) main.o parser.o router.o coordinate.o -o lab3 
main.o: main.cpp
	$(CXX) $(LINKFLAGS) -DCOMPILETIME="\"`date`\"" main.cpp -c  
parser.o: parser.cpp parser.h
	$(CXX) $(LINKFLAGS) parser.cpp -c  
router.o: router.cpp router.h
	$(CXX) $(LINKFLAGS) router.cpp -c  
coordinate.o: coordinate.cpp coordinate.h
	$(CXX) $(LINKFLAGS) coordinate.cpp -c  

test:
	./lab3 ./case1/test.in

test2:
	./lab3 ./case2/case2.in

runall:
	./lab3 case1.in case1.out
	./lab3 case2.in case2.out
	./lab3 case3.in case3.out
	./lab3 case4.in case4.out
	./lab3 case5.in case5.out

testall:
	./draw case1.in case1.out
	./draw case2.in case2.out
	./draw case3.in case3.out
	./draw case4.in case4.out
	./draw case5.in case5.out

clean:
	rm -rf *.o *.gch Lab3






