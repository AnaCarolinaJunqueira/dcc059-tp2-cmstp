#include "Grafo.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

Grafo::Grafo(){
    n = 0;
    deposito = -1;
    capacidade = 0;
    //mediaSolucoes = 0;
    //melhorAlphaReativo = -1;
}

void Grafo::criarGrafo(int quantidadedeVertices)
{
    n = quantidadedeVertices;
    matriz.assign(n, vector<double>(n,0));
    demanda.assign(n, 0); 
}

void Grafo::setDeposito(int d){
    deposito = d;
}

void Grafo::setCapacidade(int q){
    capacidade = q;
}

void Grafo::setDemanda(int vertice, int peso){
    demanda[vertice] = peso;
}

void Grafo::inserirAresta(int u, int v, double custo){
    matriz[u][v] = custo;
    matriz[v][u] = custo;
}

double Grafo::getCusto(int u, int v){
    return matriz[u][v];
}

int Grafo::getDemanda(int v){
    return demanda[v];
}


int Grafo::getNumeroVertices()
{
    return n;
}

int Grafo::getDeposito()
{
    return deposito;
}

int Grafo::getCapacidade()
{
    return capacidade;
}

Solucao& Grafo::getSolucao()
{
    return solucao;
}
