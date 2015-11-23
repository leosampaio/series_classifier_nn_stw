objs = main.o 
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
exe = P2

all : $(objs)
	$(CXX) $(CXXFLAGS) $(objs) -o $(exe)

debug_run :
	@gdb ./$(exe)

run :
	@./$(exe)

clean :
	$(RM) $(objs) $(exe) *~
