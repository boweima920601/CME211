# conventional variable for c++ compiler
CXX := g++

# conventional variable for C++ compiler flags
CXXFLAGS := -O3 -std=c++11 -Wall -Wextra -Wconversion

# conventional variable for linker flags
LDFLAGS := -ljpeg

main: main.cpp image.cpp image.hpp hw6.cpp hw6.hpp
	$(CXX) $(CXXFLAGS) -o main main.cpp hw6.cpp image.cpp $(LDFLAGS)

# here is a target to clean up the output of the build process
.PHONY: clean
clean:
	$(RM) main