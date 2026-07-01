#include "Grafo.hpp"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

Grafo::Grafo(){
    n = 0;
    deposito = -1;
    capacidade = 0;
}

void Grafo::criarGrafo(int quantidadedeVertices){
    n = quantidadedeVertices;
    matriz.assign(n, vector<double>(n,0));
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

/*//Heuristica gulosa para o problema CMSTP
void Grafo::gulosoCMSTP(int raiz, int capacidade)
{
    //vetor para marcar quais vertices ja foram incluidos na solucao
    vector<bool> visitado(vertices.size(), false);

    //marca raiz como visitada
    visitado[raiz] = true;

    //guarda soma das demandas ja incluidas na solucao
    int demandaTotal = vertices[raiz].getDemanda();

    //estrutura auxiliar para guardar origem da construcao da arvore
    vector<int> origemNo(vertices.size(), -1);

    //loop principal que vai tentar expandir a arvore
    while(true)
    {
        //inicializa menor custo com um valor muito alto pra evitar problema na comparacao
        double menorCusto = 999999;

        //melhor aresta encontrada nesta iteracao
        int melhorOrigem = -1;
        int melhorDestino = -1;

        //for pra percorrer todos os vertices do grafo
        for(int i = 0; i < vertices.size(); i++)
        {
            //só considera vertices que ja estao na solucao
            if(!visitado[i])
                continue;

            //percorre todas as arestas saindo do vertice atual
            for(int j = 0; j < adj[i].size(); j++)
            {
                //pega o vertice destino da aresta
                int destino = adj[i][j].getDestino();

                //se destino ja foi visitado ignora
                if(visitado[destino])
                    continue;

                //pega a demanda do vertice destino
                int demanda = vertices[destino].getDemanda();

                //verifica capacidade
                if(demandaTotal + demanda > capacidade)
                    continue;

                //escolhe menor custo
                if(adj[i][j].getCusto() < menorCusto)
                {
                    menorCusto = adj[i][j].getCusto();
                    melhorOrigem = i;
                    melhorDestino = destino;
                }
            }
        }

        //se nao achou nenhuma aresta valida, encerra
        if(melhorDestino == -1 || melhorOrigem == -1)
            break;

        //marca vertice como incluido
        visitado[melhorDestino] = true;

        //registra origem
        origemNo[melhorDestino] = melhorOrigem;

        //atualiza demanda
        demandaTotal += vertices[melhorDestino].getDemanda();
    }
}*/