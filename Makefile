CXX = g++
CXXFLAGS = -Wall -g

default : solver

solver : sudoku_solver.o 
	$(CXX) $(CXXFLAGS) -o solve sudoku_solver.o 
	rm sudoku_solver.o

sudoku_solver.o :
	$(CXX) $(CXXFLAGS) -c sudoku_solver.cpp 

clean : 
	rm *.o
	rm ./solve
	rm ./a.out
