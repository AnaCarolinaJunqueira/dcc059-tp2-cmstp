#include "Solucao.hpp"

#include <iostream>

using namespace std;

Solucao::Solucao()
{
    mediaSolucoes = 0;
    melhorAlphaReativo = -1;
}

void Solucao::setArestasMelhorSolucao(vector<pair<int, int>> arestas)
{
    arestasMelhorSolucao = arestas;
}

void Solucao::setMediaSolucoes(double media)
{
    mediaSolucoes = media;
}

void Solucao::setMelhorAlphaReativo(double alpha)
{
    melhorAlphaReativo = alpha;
}

vector<pair<int, int>> Solucao::getArestasMelhorSolucao()
{
    return arestasMelhorSolucao;
}

double Solucao::getMediaSolucoes()
{
    return mediaSolucoes;
}

double Solucao::getMelhorAlphaReativo()
{
    return melhorAlphaReativo;
}

void Solucao::imprimirSolucao()
{
    if (arestasMelhorSolucao.empty())
    {
        cout << "Nenhuma solucao valida encontrada." << endl;
        return;
    }

    cout << "Arestas da solucao (origem destino):" << endl;

    for (int i = 0; i < (int)arestasMelhorSolucao.size(); i++)
    {
        cout << arestasMelhorSolucao[i].first << " "
             << arestasMelhorSolucao[i].second << endl;
    }
}