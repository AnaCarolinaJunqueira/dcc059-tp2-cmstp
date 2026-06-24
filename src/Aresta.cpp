#include "Aresta.hpp"
#include <iostream>

using namespace std;

Aresta::Aresta(int origem, int destino, double custo){
    this->origem = origem;
    this->destino = destino;
    this->custo = custo;
}

int Aresta::getOrigem(){
    return origem;
}

int Aresta::getDestino(){
    return destino;
}

double Aresta::getCusto(){
    return custo;
}