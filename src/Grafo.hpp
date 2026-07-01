#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include <iostream>

using namespace std;

class Grafo
{
    private:
        //quantidade de vertices
        int n;

        //vertice raiz
        int deposito;

        //capacidade Q
        int capacidade;

        //demanda de cada vertice
        vector<int> demanda;

        //matriz de custos
        vector<vector<double>> matriz;

    public:
        Grafo();

        //criação dos vertices
        void criarGrafo(int quantidadedeVertices);

        void setDeposito(int d);
        void setCapacidade(int q);
        void setDemanda(int vertice, int peso);

        void inserirAresta(int u, int v, double custo);

        double getCusto(int u, int v);
        int getDemanda(int v);
};

#endif