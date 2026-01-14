CXX = g++
CXXFLAGS = -O2 -std=c++17

SRC = src/main.cpp src/Graph.cpp src/DefectiveColoring.cpp
OUT = defective

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f defective
