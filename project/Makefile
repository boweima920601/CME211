# conventional variable for c++ compiler

CXX := g++

# conventional variable for C++ compiler flags
CXXFLAGS := -O3 -std=c++11 -Wall -Wextra -Wconversion
RM := rm
#TARGET := main
#OBJECTS = main.o COO2CSR.o CGSolver.o matvecops.o heat.o sparse.o
#INCS = COO2CSR.hpp CGSolver.hpp matvecops.hpp heat.hpp sparse.hpp

main: main.cpp COO2CSR.cpp COO2CSR.hpp CGSolver.cpp CGSolver.hpp matvecops.cpp matvecops.hpp heat.cpp heat.hpp sparse.cpp sparse.hpp 
	$(CXX) $(CXXFLAGS) -o main main.cpp COO2CSR.cpp CGSolver.cpp matvecops.cpp heat.cpp sparse.cpp

# $(TARGET): $(OBJS)
#     $(CXX) -o $(TARGET) $(OBJS)

# # this is a make pattern rule
# %.o: %.cpp $(INCS)
#     $(CXX) -c -o $@ $< $(CXXFLAGS)

# here is a target to clean up the output of the build process
.PHONY: clean
clean:
	$(RM) main
