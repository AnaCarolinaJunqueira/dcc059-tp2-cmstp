#include "Grafo.hpp"
#include <algorithm>
#include <iostream>

using namespace std;

Grafo::Grafo(){
}

void Grafo::adicionarVertice(int id, int demanda){
    vertices.push_back(Vertice(id, demanda));

    //aq cria uma lista de adj vazia pro novo vertice
    adj.push_back(vector<Aresta>());
}

void Grafo::removerVertice(int id){
    //verifica se o vertice existe
    if(id < 0 || id >= vertices.size()){
        return;
    }

    //Aq marca o vertice como removido
    vertices[id].desativar();

    //remove todas as arestas que saem dele
    adj[id].clear();

    //remove todas as arestas que chegam nele
    for(int i = 0; i < adj.size(); i++){
        removerAresta(i, id);
    }
}

void Grafo::adicionarAresta(int origem, int destino, double custo){
    adj[origem].push_back(
        Aresta(origem, destino, custo) 
    );

    adj[destino].push_back(
        Aresta(destino, origem, custo)
    );
}

void Grafo::removerAresta(int origem, int destino){

    //remove a aresta origem ---> destino
    for(int i = 0; i < adj[origem].size(); i++){
        if(adj[origem][i].getDestino() == destino){
            adj[origem].erase(adj[origem].begin() + i);
            break;
        }
    }

    //remove a aresta destino ---> origem
    for(int i = 0; i < adj[destino].size(); i++){
        if(adj[destino][i].getDestino() == origem){
            adj[destino].erase(adj[destino].begin() + i);
            break;
        }
    }
}

void Grafo::imprimirGrafo(){
    //indica a quantidade de vértices
    cout << vertices.size() << endl;

    //vertices
    for(int i = 0; i < vertices.size(); i++){
        cout << vertices[i].getId() << endl;
    }

    //arestas
    for(int i = 0; i < adj.size(); i++){
        for(int j = 0; j < adj[i].size(); j++){
            if(adj[i][j].getOrigem() < adj[i][j].getDestino()){
                cout << adj[i][j].getOrigem() << " " << adj[i][j].getDestino() << endl;
            }
        }
    }
}

//Heuristica gulosa para o problema CMSTP
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
}