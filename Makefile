CXX = g++
CXXFLAGS = -std=c++17 -O2
TARGET = cmstp
SRC = src/main.cpp src/Grafo.cpp

all: $(TARGET)

$(TARGET): $(SRC) src/Grafo.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) resultados.csv

.PHONY: all clean
