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


double Algoritmos::gulosoRandomizadoCMSTP(Grafo& grafo, double alpha, int numIteracoes) {
    int n = grafo.getNumeroVertices();
    double melhorCustoGeral = 99999999.0;
    double somaSolucoes = 0.0;
    int qtdSolucoes = 0;

    // Estrutura simples para organizar as arestas candidatas
    struct ArestaCandidata {
        int origem;
        int destino;
        double custo;
    };

    // O algoritmo roda várias vezes para tentar achar a melhor combinação (o professor exige pelo menos 30)
    for (int iter = 0; iter < numIteracoes; iter++) {
        vector<bool> visitado(n, false);
        visitado[grafo.getDeposito()] = true;

        vector<int> idRamo(n, -1);
        vector<int> cargaRamo(n, 0);
        vector<pair<int, int>> arestasAtual;

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
                    
                    double custo = grafo.getCusto(i, j);
                    if (custo <= 0) continue; 

                    int demandaDestino = grafo.getDemanda(j);
                    bool ehValida = false;

                    if (i == grafo.getDeposito()) {
                        if (demandaDestino <= grafo.getCapacidade()) ehValida = true;
                    } else {
                        int idSubarvoreAtual = idRamo[i];
                        if (cargaRamo[idSubarvoreAtual] + demandaDestino <= grafo.getCapacidade()) ehValida = true;
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
            arestasAtual.push_back({escolhida.origem, escolhida.destino});

            if (escolhida.origem == grafo.getDeposito()) {
                idRamo[escolhida.destino] = escolhida.destino;
                cargaRamo[escolhida.destino] = grafo.getDemanda(escolhida.destino);
            } else {
                int subarvore = idRamo[escolhida.origem];
                idRamo[escolhida.destino] = subarvore;
                cargaRamo[subarvore] += grafo.getDemanda(escolhida.destino);
            }
        }

        // Se conectou todo mundo e o custo foi o menor visto até agora, salva
        if (solucaoValida) {
            somaSolucoes += custoTotal;
            qtdSolucoes++;

            if (custoTotal < melhorCustoGeral) {
                melhorCustoGeral = custoTotal;
                grafo.setArestasMelhorSolucao(arestasAtual);
            }
        }
    }

    if (qtdSolucoes > 0) {
        grafo.setMediaSolucoes(somaSolucoes / qtdSolucoes);
    } else {
        grafo.setMediaSolucoes(0);

    return melhorCustoGeral;
}