#include "Solucao.hpp"

#include <iostream>

using namespace std;

Solucao::Solucao()
{
    custo = 0;
    mediaSolucoes = 0;
    melhorAlphaReativo = -1;
}

void Solucao::setArestas(const vector<pair<int, int>>& novasArestas)
{
    arestas = novasArestas;
}

void Solucao::adicionarAresta(int origem, int destino)
{
    arestas.push_back({origem, destino});
}

void Solucao::setCusto(double novoCusto)
{
    custo = novoCusto;
}

void Solucao::setMediaSolucoes(double media)
{
    mediaSolucoes = media;
}

void Solucao::setMelhorAlphaReativo(double alpha)
{
    melhorAlphaReativo = alpha;
}

vector<pair<int, int>> Solucao::getArestas() const
{
    return arestas;
}

double Solucao::getCusto() const
{
    return custo;
}

double Solucao::getMediaSolucoes() const
{
    return mediaSolucoes;
}

double Solucao::getMelhorAlphaReativo() const
{
    return melhorAlphaReativo;
}

void Solucao::imprimirSolucao() const
{
    if (arestas.empty())
    {
        cout << "Nenhuma solucao valida encontrada." << endl;
        return;
    }

    cout << "Arestas da solucao (origem destino):" << endl;

    for (int i = 0; i < (int)arestas.size(); i++)
    {
        cout << arestas[i].first << " " << arestas[i].second << endl;
    }
}