#ifndef SOLUCAO_HPP
#define SOLUCAO_HPP

#include <vector>
#include <utility>


using namespace std;

class Solucao
{
private:
    vector<pair<int, int>> arestasMelhorSolucao;
    double mediaSolucoes;
    double melhorAlphaReativo;

public:
    Solucao();

    void setArestasMelhorSolucao(vector<pair<int, int>> arestas);
    void setMediaSolucoes(double media);
    void setMelhorAlphaReativo(double alpha);

    vector<pair<int, int>> getArestasMelhorSolucao();
    double getMediaSolucoes();
    double getMelhorAlphaReativo();

    void imprimirSolucao();
};

#endif