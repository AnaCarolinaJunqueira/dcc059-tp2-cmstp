#include "Grafo.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

Grafo::Grafo(){
    n = 0;
    deposito = -1;
    capacidade = 0;
}

void Grafo::criarGrafo(int quantidadedeVertices)
{
    n = quantidadedeVertices;

    matriz.assign(n, vector<double>(n, 0));
    demanda.assign(n, 0);
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

bool Grafo::lerInstancia(string nomeArquivo)
{
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo!" << endl;
        return false;
    }

    string nomeInstancia;
    int quantidadeClientes;

    // Lê o nome da instância
    arquivo >> nomeInstancia;

    // Lê a quantidade de clientes
    arquivo >> quantidadeClientes;

    // Cria o grafo (clientes + depósito)
    criarGrafo(quantidadeClientes + 1);

    // O depósito é o último vértice
    deposito = quantidadeClientes;

    // Nas instâncias tc e te todos os clientes têm demanda 1
    for (int i = 0; i < quantidadeClientes; i++)
    {
        demanda[i] = 1;
    }

    // O depósito possui demanda 0
    demanda[deposito] = 0;

    //Se for uma instancia CM, ele vai ler automaticament o arquivo priz
    if(nomeInstancia.substr(0,2) == "cm"){

        //Descobre se o tamanho é 50, 100 ou 200
        string tamanho;

        for(int i = 2; isdigit(nomeInstancia[i]); i++){
            tamanho += nomeInstancia[i];
        }

        string arquivoPriz = "priz" + tamanho + "r.dat";

        ifstream arquivoDemandas(arquivoPriz);

        if(!arquivoDemandas.is_open()){
            cout << "Erro ao abrir o arquivo de damandas: " << arquivoPriz << endl;
            return false;
        }

        string lixo;
        arquivoDemandas >> lixo; //pula "priz50r.dat"

        for(int i = 0; i < quantidadeClientes; i++){
            arquivoDemandas >> demanda[i];
        }

        demanda[deposito] = 0;
        arquivoDemandas.close();
    }

    // Lê a matriz de custos
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double custo;
            arquivo >> custo;

            inserirAresta(i, j, custo);
        }
    }

    arquivo.close();

    return true;
}