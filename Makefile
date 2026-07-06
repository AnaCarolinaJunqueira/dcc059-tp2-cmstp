CXX = g++
CXXFLAGS = -std=c++17 -O2

TARGET = cmstp
TARGET_EXP = experimentos

SRC = src/Grafo.cpp src/LeitorInstancia.cpp src/Algoritmos.cpp src/Solucao.cpp src/Experimentos.cpp

all: $(TARGET) $(TARGET_EXP)

$(TARGET): src/main.cpp $(SRC) src/Grafo.hpp src/LeitorInstancia.hpp src/Algoritmos.hpp src/Solucao.hpp src/Experimentos.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) src/main.cpp $(SRC)

$(TARGET_EXP): src/main_experimentos.cpp $(SRC) src/Grafo.hpp src/LeitorInstancia.hpp src/Algoritmos.hpp src/Solucao.hpp src/Experimentos.hpp
	$(CXX) $(CXXFLAGS) -o $(TARGET_EXP) src/main_experimentos.cpp $(SRC)

clean:
	rm -f $(TARGET) $(TARGET_EXP) resultados.csv

.PHONY: all clean