#include "Algoritmos.hpp"

#include <algorithm>
#include <cmath>

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

        for (int i = 0; i < n; i++) {
            if (!visitado[i]) continue; 

            for (int j = 0; j < n; j++) {
                if (visitado[j]) continue;  
                
                double custo = grafo.getCusto(i, j);
                if (custo == 0) continue;

                int demandaDestino = grafo.getDemanda(j);

                if (i == grafo.getDeposito()) {
                    if (demandaDestino <= grafo.getCapacidade() && custo < menorCusto) {
                        menorCusto = custo;
                        melhorOrigem = i;
                        melhorDestino = j;
                    }
                } 

                else {
                    int idSubarvoreAtual = idRamo[i];

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


        if (melhorOrigem == -1 || melhorDestino == -1) {
            cout << "ERRO: Trava de capacidade. Nao foi possivel conectar com Q = " << grafo.getCapacidade() << endl;
            break;
        }

        visitado[melhorDestino] = true;
        custoTotal += menorCusto;
        verticesConectados++;
        arestasAtual.push_back({melhorOrigem, melhorDestino});


        if (melhorOrigem == grafo.getDeposito()) {
            idRamo[melhorDestino] = melhorDestino; 
            cargaRamo[melhorDestino] = grafo.getDemanda(melhorDestino);
        } else {
            int subarvoreDaOrigem = idRamo[melhorOrigem];
            idRamo[melhorDestino] = subarvoreDaOrigem;
           cargaRamo[subarvoreDaOrigem] += grafo.getDemanda(melhorDestino);
        }
    }


    if (verticesConectados == n) {
        grafo.getSolucao().setArestasMelhorSolucao(arestasAtual);
    }
    
    return custoTotal;
} 


double Algoritmos::gulosoRandomizadoCMSTP(Grafo& grafo, double alpha, int numIteracoes) {
    int n = grafo.getNumeroVertices();
    double melhorCustoGeral = 99999999.0;
    double somaSolucoes = 0.0;
    int qtdSolucoes = 0;

    struct ArestaCandidata {
        int origem;
        int destino;
        double custo;
    };

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

                    if (ehValida) {
                        candidatas.push_back({i, j, custo});
                        if (custo < menorCustoValido) menorCustoValido = custo;
                        if (custo > maiorCustoValido) maiorCustoValido = custo;
                    }
                }
            }

            if (candidatas.empty()) {
                solucaoValida = false; 
                break;
            }

            sort(candidatas.begin(), candidatas.end(),
                [](const ArestaCandidata& a, const ArestaCandidata& b)
                {
                    return a.custo < b.custo;
                }
            );

            int tamanhoLRC = max(1, (int)ceil(alpha * candidatas.size()));

            int indiceSorteado = rand() % tamanhoLRC;
            ArestaCandidata escolhida = candidatas[indiceSorteado];

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

        if (solucaoValida) {
            somaSolucoes += custoTotal;
            qtdSolucoes++;

            if (custoTotal < melhorCustoGeral) {
                melhorCustoGeral = custoTotal;
                grafo.getSolucao().setArestasMelhorSolucao(arestasAtual);
            }
        }
    }

    if (qtdSolucoes > 0) {
        grafo.getSolucao().setMediaSolucoes(somaSolucoes / qtdSolucoes);
    } else {
        grafo.getSolucao().setMediaSolucoes(0);
    }
    return melhorCustoGeral;
}

double Algoritmos::gulosoRandomizadoReativoCMSTP(Grafo& grafo,const vector<double>& alphas,int numIteracoes,int tamanhoBloco){
    int n = grafo.getNumeroVertices();
    int quantidadeAlphas = alphas.size();

    // Todos os alfas comecam com a mesma chance
    vector<double> probabilidades(quantidadeAlphas, 1.0 / quantidadeAlphas);
    vector<double> somaCustos(quantidadeAlphas, 0.0);
    vector<int> contagem(quantidadeAlphas, 0);

    double melhorCustoGeral = 99999999.0;
    double somaSolucoes = 0.0;
    int qtdSolucoes = 0;
    grafo.getSolucao().setMelhorAlphaReativo(-1);

    for (int iter = 0; iter < numIteracoes; iter++) {
        double sorteio = (double)rand() / RAND_MAX;
        double acumulado = 0.0;
        int indiceAlpha = 0;

        for (int j = 0; j < quantidadeAlphas; j++) {
            acumulado += probabilidades[j];
            if (sorteio <= acumulado) {
                indiceAlpha = j;
                break;
            }
        }

        vector<pair<int, int>> arvoreAnterior = grafo.getSolucao().getArestasMelhorSolucao();
        double custoTotal = gulosoRandomizadoCMSTP(grafo, alphas[indiceAlpha], 1);
        if (custoTotal >= 99999999.0) {
            continue;
        }

        somaSolucoes += custoTotal;
        qtdSolucoes++;

        if (custoTotal < melhorCustoGeral) {
            melhorCustoGeral = custoTotal;
            grafo.getSolucao().setMelhorAlphaReativo(alphas[indiceAlpha]);
        } else {
            grafo.getSolucao().setArestasMelhorSolucao(arvoreAnterior);
        }

        somaCustos[indiceAlpha] += custoTotal;
        contagem[indiceAlpha]++;

        bool fimDoBloco = ((iter + 1) % tamanhoBloco == 0) || (iter == numIteracoes - 1);
        if (!fimDoBloco) {
            continue;
        }

        vector<double> novasProbabilidades(quantidadeAlphas, 0.0);
        double somaInversas = 0.0;

        for (int j = 0; j < quantidadeAlphas; j++) {
            if (contagem[j] > 0) {
                double mediaCusto = somaCustos[j] / contagem[j];
                novasProbabilidades[j] = 1.0 / mediaCusto;
                somaInversas += novasProbabilidades[j];
            } else {
                novasProbabilidades[j] = probabilidades[j];
                somaInversas += novasProbabilidades[j];
            }
        }

        if (somaInversas > 0) {
            for (int j = 0; j < quantidadeAlphas; j++) {
                probabilidades[j] = novasProbabilidades[j] / somaInversas;
            }
        }

        somaCustos.assign(quantidadeAlphas, 0.0);
        contagem.assign(quantidadeAlphas, 0);
    }

    if (qtdSolucoes > 0) {
        grafo.getSolucao().setMediaSolucoes(somaSolucoes / qtdSolucoes);
    } else {
        grafo.getSolucao().setMediaSolucoes(0);
    }

    return melhorCustoGeral;
}