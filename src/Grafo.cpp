#include "Grafo.hpp"
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

Grafo::Grafo(){
    n = 0;
    deposito = -1;
    capacidade = 0;
    //mediaSolucoes = 0;
    //melhorAlphaReativo = -1;
}

void Grafo::criarGrafo(int quantidadedeVertices)
{
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

/*
bool Grafo::lerInstancia(string nomeArquivo)
{
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo!" << endl;
        return false;
    }

    int totalVertices;
    arquivo >> totalVertices;

    criarGrafo(totalVertices);

    // O depósito é o último vértice
    deposito = totalVertices - 1;

    // Nas instâncias tc e te todos os clientes têm demanda 1
    for (int i = 0; i < totalVertices; i++)
    {
        demanda[i] = 1;
    }

    // O depósito possui demanda 0
    demanda[deposito] = 0;

    size_t barra = nomeArquivo.find_last_of("/\\");
    string nomeBase = (barra != string::npos) ? nomeArquivo.substr(barra + 1) : nomeArquivo;

    // Instancia cm tem demandas no priz (detecta pelo nome do arquiv)
    bool ehInstanciaCM = nomeBase.size() >= 2 && nomeBase.substr(0, 2) == "cm";

    //Se for uma instancia CM, ele vai ler automaticament o arquivo priz
    if(ehInstanciaCM){

        // Monta o caminho do priz na mesma pasta da instancia 
        string tamanho = to_string(totalVertices);

        string diretorio;
        if(barra != string::npos){
            diretorio = nomeArquivo.substr(0, barra + 1);
        }

        string arquivoPriz = diretorio + "priz" + tamanho + "r.dat";

        ifstream arquivoDemandas(arquivoPriz);

        if(!arquivoDemandas.is_open()){
            cout << "Erro ao abrir o arquivo de damandas: " << arquivoPriz << endl;
            return false;
        }

        // Le as demandas dos clientes no priz
        for(int i = 0; i < totalVertices - 1; i++){
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

            // Na diagonal, 0 ou 1000 significa sem aresta (ignora)
            if (i == j) custo = 0;

            inserirAresta(i, j, custo);
        }
    }

    arquivo.close();

    return true;
} 
*/

/*
void Grafo::imprimirSolucao() {
    if (arestasMelhorSolucao.empty()) {
        cout << "Nenhuma solucao valida encontrada." << endl;
        return;
    }

    // Uma aresta por linha: origem destino 
    cout << "Arestas da solucao (origem destino):" << endl;
    for (int i = 0; i < (int)arestasMelhorSolucao.size(); i++) {
        cout << arestasMelhorSolucao[i].first << " " << arestasMelhorSolucao[i].second << endl;
    }
}

double Grafo::getMediaSolucoes() const {
    return mediaSolucoes;
}

double Grafo::getMelhorAlphaReativo() const {
    return melhorAlphaReativo;
}
*/

/*
double Grafo::gulosoCMSTP() {
    vector<bool> visitado(n, false);
    visitado[deposito] = true;
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
        arestasAtual.push_back({melhorOrigem, melhorDestino});

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

    // Guarda a arvore se conectou todos os vertices
    if (verticesConectados == n) {
        arestasMelhorSolucao = arestasAtual;
    }
    
    return custoTotal;
} 
*/

/*
double Grafo::gulosoRandomizadoCMSTP(double alpha, int numIteracoes) {
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
        visitado[deposito] = true;

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
            arestasAtual.push_back({escolhida.origem, escolhida.destino});

            if (escolhida.origem == deposito) {
                idRamo[escolhida.destino] = escolhida.destino;
                cargaRamo[escolhida.destino] = demanda[escolhida.destino];
            } else {
                int subarvore = idRamo[escolhida.origem];
                idRamo[escolhida.destino] = subarvore;
                cargaRamo[subarvore] += demanda[escolhida.destino];
            }
        }

        // Se conectou todo mundo e o custo foi o menor visto até agora, salva
        if (solucaoValida) {
            somaSolucoes += custoTotal;
            qtdSolucoes++;

            if (custoTotal < melhorCustoGeral) {
                melhorCustoGeral = custoTotal;
                arestasMelhorSolucao = arestasAtual;
            }
        }
    }

    if (qtdSolucoes > 0) {
        mediaSolucoes = somaSolucoes / qtdSolucoes;
    } else {
        mediaSolucoes = 0;
    }

    return melhorCustoGeral;
}
*/

/*
double Grafo::gulosoRandomizadoReativoCMSTP(const vector<double>& alphas, int numIteracoes, int tamanhoBloco) {
    int quantidadeAlphas = alphas.size();

    // Todos os alfas comecam com a mesma chance
    vector<double> probabilidades(quantidadeAlphas, 1.0 / quantidadeAlphas);
    vector<double> somaCustos(quantidadeAlphas, 0.0);
    vector<int> contagem(quantidadeAlphas, 0);

    double melhorCustoGeral = 99999999.0;
    double somaSolucoes = 0.0;
    int qtdSolucoes = 0;
    melhorAlphaReativo = -1;

    for (int iter = 0; iter < numIteracoes; iter++) {
        // Sorteia qual alfa usar nessa iteracao
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

        // Roda o randomizado uma vez com esse alfa
        vector<pair<int, int>> arvoreAnterior = arestasMelhorSolucao;
        double custoTotal = gulosoRandomizadoCMSTP(alphas[indiceAlpha], 1);
        if (custoTotal >= 99999999.0) {
            continue;
        }

        somaSolucoes += custoTotal;
        qtdSolucoes++;

        if (custoTotal < melhorCustoGeral) {
            melhorCustoGeral = custoTotal;
            melhorAlphaReativo = alphas[indiceAlpha];
        } else {
            // Descarta arvore ruim e mantem a melhor anterior
            arestasMelhorSolucao = arvoreAnterior;
        }

        // Acumula custos do bloco para recalcular probabilidades depois
        somaCustos[indiceAlpha] += custoTotal;
        contagem[indiceAlpha]++;

        bool fimDoBloco = ((iter + 1) % tamanhoBloco == 0) || (iter == numIteracoes - 1);
        if (!fimDoBloco) {
            continue;
        }

        // Atualiza probabilidades: alfa com menor media ganha mais chance
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

        // Zera contadores para o proximo bloco
        somaCustos.assign(quantidadeAlphas, 0.0);
        contagem.assign(quantidadeAlphas, 0);
    }

    if (qtdSolucoes > 0) {
        mediaSolucoes = somaSolucoes / qtdSolucoes;
    } else {
        mediaSolucoes = 0;
    }

    return melhorCustoGeral;
}
*/

int Grafo::getNumeroVertices()
{
    return n;
}

int Grafo::getDeposito()
{
    return deposito;
}

int Grafo::getCapacidade()
{
    return capacidade;
}

/*
void Grafo::setArestasMelhorSolucao(vector<pair<int, int>> arestas)
{
    arestasMelhorSolucao = arestas;
}

void Grafo::setMediaSolucoes(double media)
{
    mediaSolucoes = media;
}

void Grafo::setMelhorAlphaReativo(double alpha)
{
    melhorAlphaReativo = alpha;
}

vector<pair<int, int>> Grafo::getArestasMelhorSolucao()
{
    return arestasMelhorSolucao;
}
*/

Solucao& Grafo::getSolucao()
{
    return solucao;
}
