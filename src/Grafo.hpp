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

        // arestas da melhor solucao encontrada
        vector<pair<int, int>> arestasMelhorSolucao; //! Não faz parte do objeto grafo, ele entrega solução

        // estatisticas dos algoritmos randomizados 
        double mediaSolucoes; //! Não faz parte do objeto grafo, ele entrega solução
        double melhorAlphaReativo; //! Não faz parte do objeto grafo, ele entrega solução


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
        // bool lerInstancia(string nomeArquivo);  //! faz parte de um objeto grafo. 
        //double gulosoCMSTP(); //! É um algoritmo que recebe grafo
        //double gulosoRandomizadoCMSTP(double alpha, int numIteracoes); //! É um algoritmo que recebe grafo
        //double gulosoRandomizadoReativoCMSTP(const vector<double>& alphas, int numIteracoes, int tamanhoBloco); //! É um algoritmo que recebe grafo
        void imprimirSolucao();
        double getMediaSolucoes() const;
        double getMelhorAlphaReativo() const;

        //métodos públicos para acessar atributos privados
        int getNumeroVertices();
        int getDeposito();
        int getCapacidade();

        void setArestasMelhorSolucao(vector<pair<int, int>> arestas);
        void setMediaSolucoes(double media);
};

#endif