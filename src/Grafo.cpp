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
        Aresta(destino, custo) 
    );

    adj[destino].push_back(
        Aresta(origem, custo)
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