#include "Algoritmos.hpp"

double Algoritmos::gulosoCMSTP(Grafo& grafo) {
    int n = grafo.getNumeroVertices();
    vector<bool> visitado(grafo.getNumeroVertices(), false);
    visitado[grafo.getDeposito()] = true;
    vector<int> idRamo(n, -1);
    vector<int> cargaRamo(n, 0);
    vector<pair<int, int>> arestasAtual;
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
                
                double custo = grafo.getCusto(i, j);
                if (custo == 0) continue;

                int demandaDestino = grafo.getDemanda(j);

                // Regra 1: Aresta saindo direto do depósito 
                if (i == grafo.getDeposito()) {
                    if (demandaDestino <= grafo.getCapacidade() && custo < menorCusto) {
                        menorCusto = custo;
                        melhorOrigem = i;
                        melhorDestino = j;
                    }
                } 
                // Regra 2: Aresta saindo de um cliente 
                else {
                    int idSubarvoreAtual = idRamo[i];
                    // Verifica se a capacidade do ramo aguenta o novo cliente
                    if (cargaRamo[idSubarvoreAtual] + demandaDestino <= grafo.getCapacidade()) {
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
            cout << "ERRO: Trava de capacidade. Nao foi possivel conectar com Q = " << grafo.getCapacidade() << endl;
            break;
        }

        visitado[melhorDestino] = true;
        custoTotal += menorCusto;
        verticesConectados++;
        arestasAtual.push_back({melhorOrigem, melhorDestino});

        // Atualiza as cargas dos ramos
        if (melhorOrigem == grafo.getDeposito()) {
            idRamo[melhorDestino] = melhorDestino; 
            cargaRamo[melhorDestino] = grafo.getDemanda(melhorDestino);
        } else {
            int subarvoreDaOrigem = idRamo[melhorOrigem];
            idRamo[melhorDestino] = subarvoreDaOrigem;
           cargaRamo[subarvoreDaOrigem] += grafo.getDemanda(melhorDestino);
        }
    }

    // Guarda a arvore se conectou todos os vertices
    if (verticesConectados == n) {
        grafo.setArestasMelhorSolucao(arestasAtual);
    }
    
    return custoTotal;
} 