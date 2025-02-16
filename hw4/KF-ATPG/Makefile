# ------------------------------------
# Feng Kai Path Delay ATPG
# 
# 2004/5/25
# Kai Yang (kyang@ece.ucsb.edu)
# ------------------------------------

CC = g++ -std=c++98
#CFLAGS = -g
CFLAGS = -DNDEBUG -O3 -w
MFLAGS =
YACC = bison
LEX = flex
LFLAGS = 
YFLAGS = -v -d
LIBS = -lm
#SOLVER = -I /home/lufeng/csat_new1
#SOLVER_LIB = /home/lufeng/csat_new1
SOLVER = -I ./csat_new1
SOLVER_LIB = ./csat_new1

COMPILE_DEF = "-DCUR_DATE=\"`date`\""

HEADERS = 	GetLongOpt.h \
		  	readbench.tab.h \
			kai_gate.h \
			kai_netlist.h \
			kai_typeemu.h \
			kai_path.h \
			kai_constraint.h \
			kai_pattern.h \
			kai_objective.h


SRCS = 		readbench.cpp \
			lexbench.cpp \
			GetLongOpt.cpp \
			kai_version.cpp \
			kai_util.cpp \
			kai_netlist.cpp \
			kai_path.cpp \
			kai_constraint.cpp \
			kai_dump.cpp \
			kai_expand.cpp \
			kai_objective.cpp \
			kai_convert.cpp \
			kai_pattern.cpp \
       		kai_main.cpp

TARGET = KF_ATPG

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(SOLVER) -o $@ $(OBJS) $(LIBS) $(SOLVER_LIB)/libsat.a

%.o: %.cpp $(HEADERS) Makefile
	$(CC) $(CFLAGS) $(SOLVER) $(COMPILE_DEF) -c -o $*.o $<

readbench.cpp: readbench.y Makefile
	$(YACC) $(YFLAGS) -p kk readbench.y
	mv readbench.tab.c $@
	rm readbench.output

lexbench.cpp: lexbench.l Makefile
	$(LEX) -Pkk lexbench.l
	mv lex.kk.c $@

run1_r:
	./KF_ATPG -atpg R -cir example/c17.bench -path_not example/c17.path_not -output c17_R.pttn
run1_nr:
	./KF_ATPG -atpg NR -cir example/c17.bench -path_not example/c17.path_not -output c17_NR.pttn
run2_r:
	./KF_ATPG -atpg R -cir example/s27.bench -path_not example/s27.path_not -output s27_R.pttn
run2_nr:
	./KF_ATPG -atpg NR -cir example/s27.bench -path_not example/s27.path_not -output s27_NR.pttn

clean:
	rm *.o readbench.cpp lexbench.cpp readbench.tab.h $(TARGET)
