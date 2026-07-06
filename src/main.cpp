#include "Grafo.hpp"
#include "LeitorInstancia.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
using namespace std;

// Inicializa a semente do rand() 
unsigned int configurarSemente(int argc, char* argv[]) {
    unsigned int semente = (unsigned int)time(NULL);

    // Se passar --semente ou -s, usa o valor informado
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if ((arg == "--semente" || arg == "-s") && i + 1 < argc) {
            semente = (unsigned int)stoul(argv[i + 1]);
            i++;
        }
    }

    srand(semente);
    cout << "Semente de randomizacao: " << semente << endl;

    return semente;
}

// Pega o valor de um parametro da linha de comando
string buscarParametro(int argc, char* argv[], const string& nome, const string& padrao) {
    for (int i = 1; i < argc; i++) {
        if (nome == argv[i] && i + 1 < argc) {
            return argv[i + 1];
        }
    }
    return padrao;
}

// Converte texto tipo "0.1,0.3,0.5" em vector de alfas
vector<double> lerAlphas(const string& texto) {
    vector<double> alphas;
    string numero;

    for (int i = 0; i <= (int)texto.size(); i++) {
        if (i == (int)texto.size() || texto[i] == ',') {
            if (!numero.empty()) {
                alphas.push_back(stod(numero));
                numero.clear();
            }
        } else {
            numero += texto[i];
        }
    }

    return alphas;
}

// Retorna data e hora atuais para o CSV
string obterDataHora() {
    time_t agora = time(NULL);
    tm* info = localtime(&agora);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", info);
    return string(buffer);
}

// Cria cabecalho do CSV se o arquivo ainda nao existir
void criarCabecalhoCSV(const string& arquivo) {
    ifstream verifica(arquivo);
    if (verifica.good()) {
        verifica.close();
        return;
    }
    verifica.close();

    ofstream csv(arquivo);
    csv << "data_hora,instancia,capacidade,algoritmo,alpha,alphas,iteracoes,bloco,semente,tempo_s,melhor_custo,media_custos,melhor_alpha_reativo" << endl;
}

// Registra uma linha com os dados da execucao
void registrarCSV(const string& arquivo, const string& instancia, const string& capacidade,
                  const string& algoritmo, const string& alpha, const string& alphas,
                  const string& iteracoes, const string& bloco, unsigned int semente,
                  double tempo, double melhorCusto, const Grafo& g) {
    criarCabecalhoCSV(arquivo);

    ofstream csv(arquivo, ios::app);
    csv << fixed << setprecision(4);
    csv << obterDataHora() << ","
        << instancia << ","
        << capacidade << ","
        << algoritmo << ","
        << alpha << ",";
    if (!alphas.empty()) csv << "\"" << alphas << "\",";
    else csv << ",";
    csv << iteracoes << ","
        << bloco << ","
        << semente << ","
        << tempo << ","
        << melhorCusto << ",";

    if (algoritmo == "randomizado" || algoritmo == "reativo") {
        csv << g.getMediaSolucoes() << ",";
    } else {
        csv << ",";
    }

    if (algoritmo == "reativo" && g.getMelhorAlphaReativo() >= 0) {
        csv << g.getMelhorAlphaReativo();
    }

    csv << endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "Erro: informe --algoritmo, --instancia e --capacidade." << endl;
        cout << "Veja COMO_EXECUTAR.txt" << endl;
        return 1;
    }

    string algoritmo = buscarParametro(argc, argv, "--algoritmo", "");
    string instancia = buscarParametro(argc, argv, "--instancia", "");
    string capacidadeStr = buscarParametro(argc, argv, "--capacidade", "");

    if (algoritmo.empty() || instancia.empty() || capacidadeStr.empty()) {
        cout << "Erro: informe --algoritmo, --instancia e --capacidade." << endl;
        return 1;
    }

    if (algoritmo != "guloso" && algoritmo != "randomizado" && algoritmo != "reativo") {
        cout << "Erro: algoritmo invalido. Use guloso, randomizado ou reativo." << endl;
        return 1;
    }

    unsigned int semente = configurarSemente(argc, argv);

    Grafo g;
    LeitorInstancia leitor;

    if (!leitor.lerInstancia(g, instancia)) {
        cout << "Erro ao ler a instância!" << endl;
        return 1;
    }

    g.setCapacidade(stoi(capacidadeStr));

    cout << "Instancia: " << instancia << endl;
    cout << "Capacidade Q: " << capacidadeStr << endl;

    double custo = 0;
    string alphaStr = "";
    string alphasStr = "";
    string iterStr = "";
    string blocoStr = "";

    auto inicio = chrono::high_resolution_clock::now();

    // Roda o algoritmo escolhido
    if (algoritmo == "guloso") {
        custo = g.gulosoCMSTP();
        cout << "Algoritmo: guloso" << endl;
    }
    else if (algoritmo == "randomizado") {
        double alpha = stod(buscarParametro(argc, argv, "--alpha", "0.3"));
        int iteracoes = stoi(buscarParametro(argc, argv, "--iteracoes", "30"));

        alphaStr = to_string(alpha);
        iterStr = to_string(iteracoes);

        custo = g.gulosoRandomizadoCMSTP(alpha, iteracoes);
        cout << "Algoritmo: randomizado" << endl;
        cout << "Alpha: " << alpha << " | Iteracoes: " << iteracoes << endl;
    }
    else if (algoritmo == "reativo") {
        alphasStr = buscarParametro(argc, argv, "--alphas", "0.1,0.3,0.5");
        vector<double> alphas = lerAlphas(alphasStr);
        int iteracoes = stoi(buscarParametro(argc, argv, "--iteracoes", "300"));
        int bloco = stoi(buscarParametro(argc, argv, "--bloco", "30"));

        iterStr = to_string(iteracoes);
        blocoStr = to_string(bloco);

        custo = g.gulosoRandomizadoReativoCMSTP(alphas, iteracoes, bloco);
        cout << "Algoritmo: reativo" << endl;
        cout << "Iteracoes: " << iteracoes << " | Bloco: " << bloco << endl;
    }
    else {
        cout << "Erro interno: algoritmo nao tratado." << endl;
        return 1;
    }

    auto fim = chrono::high_resolution_clock::now();
    double tempo = chrono::duration<double>(fim - inicio).count();

    cout << fixed << setprecision(4);
    cout << "Melhor custo: " << custo << endl;
    cout << "Tempo: " << tempo << " s" << endl;

    registrarCSV("resultados.csv", instancia, capacidadeStr, algoritmo, alphaStr, alphasStr,
                 iterStr, blocoStr, semente, tempo, custo, g);

    g.imprimirSolucao();

    return 0;
}