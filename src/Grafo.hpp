#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <iostream>
#include "Solucao.hpp"

using namespace std;

class Grafo
{
    private:

        int n;
        int deposito;
        int capacidade;
        vector<int> demanda;
        vector<vector<double>> matriz;

        Solucao solucao;


    public:
        Grafo();

        void criarGrafo(int quantidadedeVertices);

        void setDeposito(int d);
        void setCapacidade(int q);
        void setDemanda(int vertice, int peso);

        void inserirAresta(int u, int v, double custo);

        double getCusto(int u, int v);
        int getDemanda(int v);

        int getNumeroVertices();
        int getDeposito();
        int getCapacidade();

        Solucao& getSolucao();
};

#endif