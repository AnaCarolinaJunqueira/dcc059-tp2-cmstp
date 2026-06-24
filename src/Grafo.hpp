#ifndef GRAFO_HPP
#define GRAFO_HPP

#include <vector>
#include "Vertice.hpp"
#include "Aresta.hpp"
#include <iostream>

using namespace std;

class Grafo
{
    private:
        //lista de vertices do grafo
        vector<Vertice> vertices;

        //lista de adjacência
        vector<vector<Aresta>> adj;

    public:
        Grafo();

        void adicionarVertice(int id, int demanda);
        void removerVertice(int id);

        void adicionarAresta(int origem, int destino, double custo);
        void removerAresta(int origem, int destino);

        void imprimirGrafo();//falta implementar esse daqui
};

#endif