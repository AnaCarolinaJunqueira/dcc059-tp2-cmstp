#include "Grafo.hpp"
#include "LeitorInstancia.hpp"
#include "Algoritmos.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iomanip>
using namespace std;

unsigned int configurarSemente(int argc, char* argv[]) {
    unsigned int semente = (unsigned int)time(NULL);

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

string buscarParametro(int argc, char* argv[], const string& nome, const string& padrao) {
    for (int i = 1; i < argc; i++) {
        if (nome == argv[i] && i + 1 < argc) {
            return argv[i + 1];
        }
    }
    return padrao;
}

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

string obterDataHora() {
    time_t agora = time(NULL);
    tm* info = localtime(&agora);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", info);
    return string(buffer);
}

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

void registrarCSV(const string& arquivo, const string& instancia, const string& capacidade,
                  const string& algoritmo, const string& alpha, const string& alphas,
                  const string& iteracoes, const string& bloco, unsigned int semente,
                  double tempo, double melhorCusto, Grafo& g) {
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
        csv << g.getSolucao().getMediaSolucoes() << ",";
    } else {
        csv << ",";
    }

    if (algoritmo == "reativo" && g.getSolucao().getMelhorAlphaReativo() >= 0) {
        csv << g.getSolucao().getMelhorAlphaReativo();
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
    Algoritmos algoritmos;

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

    if (algoritmo == "guloso") {
        custo = algoritmos.gulosoCMSTP(g);
        cout << "Algoritmo: guloso" << endl;
    }
    else if (algoritmo == "randomizado") {
        double alpha = stod(buscarParametro(argc, argv, "--alpha", "0.3"));
        int iteracoes = stoi(buscarParametro(argc, argv, "--iteracoes", "30"));

        alphaStr = to_string(alpha);
        iterStr = to_string(iteracoes);

        custo = algoritmos.gulosoRandomizadoCMSTP(g, alpha, iteracoes);
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

        custo = algoritmos.gulosoRandomizadoReativoCMSTP(g, alphas, iteracoes, bloco);
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

    g.getSolucao().imprimirSolucao();

    return 0;
}