#include "Aresta.hpp"
#include <iostream>

using namespace std;

Aresta::Aresta(int destino, double custo){
    this->destino = destino;
    this->custo = custo;
}

int Aresta::getDestino(){
    return destino;
}

double Aresta::getCusto(){
    return custo;
}