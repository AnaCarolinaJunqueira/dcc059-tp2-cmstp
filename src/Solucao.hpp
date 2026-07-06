#ifndef SOLUCAO_HPP
#define SOLUCAO_HPP

#include <vector>
#include <utility>

using namespace std;

class Solucao
{
    private:
        vector<pair<int, int>> arestas;
        double custo;
        double mediaSolucoes;
        double melhorAlphaReativo;

    public:
        Solucao();

        void setArestas(const vector<pair<int, int>>& novasArestas);
        void adicionarAresta(int origem, int destino);

        void setCusto(double novoCusto);
        void setMediaSolucoes(double media);
        void setMelhorAlphaReativo(double alpha);

        vector<pair<int, int>> getArestas() const;
        double getCusto() const;
        double getMediaSolucoes() const;
        double getMelhorAlphaReativo() const;

        void imprimirSolucao() const;
};

#endif