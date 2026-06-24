#include "Vertice.hpp"

#include <iostream>

using namespace std;

Vertice::Vertice(int id, int demanda){
    this->id = id;
    this->demanda = demanda;
    this->ativo = true;
}

int Vertice::getId(){
    return id;
}

int Vertice::getDemanda(){
    return demanda;
}

void Vertice::setDemanda(int demanda){
    this->demanda = demanda;
}

bool Vertice::estaAtivo(){
    return ativo;
}

void Vertice::desativar(){
    ativo = false;
}