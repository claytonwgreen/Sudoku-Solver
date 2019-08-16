CXX := g++
CXXFLAGS := -Wall -g
CVC := `pkg-config --cflags opencv`
CVL :=`pkg-config --libs opencv`

default : solver

solver : sudoku_solver.o 
	$(CXX) $(CXXFLAGS) -o solve sudoku_solver.o 
	rm sudoku_solver.o

sudoku_solver.o :
	$(CXX) $(CXXFLAGS) -c sudoku_solver.cpp 

clean : 
	rm -f *.o
	rm -f ./solve
	rm -f ./a.out
	rm -f detector

detector.o:
	$(CXX) $(CXXFLAGS) $(CVC) -c detector.cpp

digits.o:
	$(CXX) $(CXXFLAGS) $(CVC) -c digits.cpp

detector: detector.o
	$(CXX) $(CXXFLAGS) $(CVL) -o detector detector.o
	rm detector.o

both: detector.o digits.o
	$(CXX) $(CXXFLAGS) $(CVL) -o detector detector.o digits.o
	rm detector.o
	rm digits.o
