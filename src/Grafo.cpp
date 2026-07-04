#include "Grafo.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

Grafo::Grafo(){
    n = 0;
    deposito = -1;
    capacidade = 0;
}

void Grafo::criarGrafo(int quantidadedeVertices){
    n = quantidadedeVertices;
    matriz.assign(n, vector<double>(n,0));
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

double Grafo::gulosoCMSTP() {
    vector<bool> visitado(n, false);
    visitado[deposito] = true;
    vector<int> idRamo(n, -1);
    vector<int> cargaRamo(n, 0);
    double custoTotal = 0.0;
    int verticesConectados = 1; 

    while (verticesConectados < n) {
        double menorCusto = 99999999.0;
        int melhorOrigem = -1;
        int melhorDestino = -1;

        // Procura em toda a matriz a melhor aresta possível
        for (int i = 0; i < n; i++) {
            if (!visitado[i]) continue; 

            for (int j = 0; j < n; j++) {
                if (visitado[j]) continue;  
                
                double custo = matriz[i][j];
                if (custo == 0) continue;

                int demandaDestino = demanda[j];

                // Regra 1: Aresta saindo direto do depósito 
                if (i == deposito) {
                    if (demandaDestino <= capacidade && custo < menorCusto) {
                        menorCusto = custo;
                        melhorOrigem = i;
                        melhorDestino = j;
                    }
                } 
                // Regra 2: Aresta saindo de um cliente 
                else {
                    int idSubarvoreAtual = idRamo[i];
                    // Verifica se a capacidade do ramo aguenta o novo cliente
                    if (cargaRamo[idSubarvoreAtual] + demandaDestino <= capacidade) {
                        if (custo < menorCusto) {
                            menorCusto = custo;
                            melhorOrigem = i;
                            melhorDestino = j;
                        }
                    }
                }
            }
        }

        // Trava de segurança
        if (melhorOrigem == -1 || melhorDestino == -1) {
            cout << "ERRO: Trava de capacidade. Nao foi possivel conectar com Q = " << capacidade << endl;
            break;
        }

        visitado[melhorDestino] = true;
        custoTotal += menorCusto;
        verticesConectados++;

        // Atualiza as cargas dos ramos
        if (melhorOrigem == deposito) {
            idRamo[melhorDestino] = melhorDestino; 
            cargaRamo[melhorDestino] = demanda[melhorDestino];
        } else {
            int subarvoreDaOrigem = idRamo[melhorOrigem];
            idRamo[melhorDestino] = subarvoreDaOrigem;
            cargaRamo[subarvoreDaOrigem] += demanda[melhorDestino];
        }
    }
    
    return custoTotal;
}

double Grafo::gulosoRandomizadoCMSTP(double alpha, int numIteracoes) {
    double melhorCustoGeral = 99999999.0;

    // Estrutura simples para organizar as arestas candidatas
    struct ArestaCandidata {
        int origem;
        int destino;
        double custo;
    };

    // O algoritmo roda várias vezes para tentar achar a melhor combinação (o professor exige pelo menos 30)
    for (int iter = 0; iter < numIteracoes; iter++) {
        vector<bool> visitado(n, false);
        visitado[deposito] = true;

        vector<int> idRamo(n, -1);
        vector<int> cargaRamo(n, 0);

        double custoTotal = 0.0;
        int verticesConectados = 1;
        bool solucaoValida = true;

        while (verticesConectados < n) {
            vector<ArestaCandidata> candidatas;
            double menorCustoValido = 99999999.0;
            double maiorCustoValido = -1.0;

            // Encontra todas as arestas válidas 
            for (int i = 0; i < n; i++) {
                if (!visitado[i]) continue;

                for (int j = 0; j < n; j++) {
                    if (visitado[j]) continue;
                    
                    double custo = matriz[i][j];
                    if (custo <= 0) continue; 

                    int demandaDestino = demanda[j];
                    bool ehValida = false;

                    if (i == deposito) {
                        if (demandaDestino <= capacidade) ehValida = true;
                    } else {
                        int idSubarvoreAtual = idRamo[i];
                        if (cargaRamo[idSubarvoreAtual] + demandaDestino <= capacidade) ehValida = true;
                    }

                    // Guarda a aresta se ela couber no ramo
                    if (ehValida) {
                        candidatas.push_back({i, j, custo});
                        if (custo < menorCustoValido) menorCustoValido = custo;
                        if (custo > maiorCustoValido) maiorCustoValido = custo;
                    }
                }
            }

            if (candidatas.empty()) {
                solucaoValida = false; // A árvore "travou" em um beco sem saída
                break;
            }

            // A Mágica do GRASP: Define o teto de custo com base no parâmetro Alfa
            double limiteCusto = menorCustoValido + alpha * (maiorCustoValido - menorCustoValido);

            // Cria a Lista Restrita de Candidatos (LRC)
            vector<ArestaCandidata> lrc;
            for (int k = 0; k < candidatas.size(); k++) {
                if (candidatas[k].custo <= limiteCusto) {
                    lrc.push_back(candidatas[k]);
                }
            }

            // Sorteia UMA aresta aleatória de dentro da lista restrita
            int indiceSorteado = rand() % lrc.size();
            ArestaCandidata escolhida = lrc[indiceSorteado];

            // Atualiza a árvore com a aresta sorteada
            visitado[escolhida.destino] = true;
            custoTotal += escolhida.custo;
            verticesConectados++;

            if (escolhida.origem == deposito) {
                idRamo[escolhida.destino] = escolhida.destino;
                cargaRamo[escolhida.destino] = demanda[escolhida.destino];
            } else {
                int subarvore = idRamo[escolhida.origem];
                idRamo[escolhida.destino] = subarvore;
                cargaRamo[subarvore] += demanda[escolhida.destino];
            }
        }

        // Se conectou todo mundo e o custo foi o menor visto até agora, salva!
        if (solucaoValida && custoTotal < melhorCustoGeral) {
            melhorCustoGeral = custoTotal;
        }
    }
    return melhorCustoGeral;
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