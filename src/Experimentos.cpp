#include "Experimentos.hpp"
#include "Grafo.hpp"
#include <iostream>
#include <filesystem>
#include <chrono>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;
namespace fs = filesystem;

Experimentos::Experimentos()
{

}

void Experimentos::executarTodos(){
    fs::create_directories("resultados_experimento");
    gerarSemente();
    executarCapmst1();
    executarCapmst2();
}

void Experimentos::executarCapmst1(){
    for (auto arquivo : fs::directory_iterator("instancias/capmst1")){
        string nome = arquivo.path().filename().string();

        vector<int> capacidade;

        if(nome.find("40") != string::npos)
            capacidade = {3,5,10};
        else
            capacidade = {5,10,20};

        for(int q : capacidade){
            executarInstancia(arquivo.path().string(), q);
        }
    }
}

void Experimentos::executarCapmst2(){
    for (auto arquivo : fs::directory_iterator("instancias/capmst2")){
        string nome = arquivo.path().filename().string();

        if(nome.find("cm") == string::npos)
            continue;
        
        vector<int> capacidades = {200, 400, 800};

        for(int q : capacidades){
            executarInstancia(arquivo.path().string(), q);
        }
    }
}

void Experimentos::executarGuloso(string arquivo, int capacidade){
    for(int i = 0; i < 10; i++){
        unsigned int sementeExecucao = semente + i;

        srand(sementeExecucao);

        Grafo g;
        g.lerInstancia(arquivo);
        g.setCapacidade(capacidade);

        auto inicio = chrono::high_resolution_clock::now();

        double custo = g.gulosoCMSTP();

        auto fim = chrono::high_resolution_clock::now();

        double tempo = chrono::duration<double>(fim-inicio).count();

        salvarResultadoCSV(arquivo, capacidade, "guloso", -1, 1, -1, sementeExecucao, tempo, custo);
    }
}

void Experimentos::executarRandomizado(string arquivo, int capacidade, double alpha, int iteracoes){
    for(int i = 0; i < 10; i++){
        unsigned int sementeExecucao = semente + i;

        srand(sementeExecucao);

        Grafo g;
        g.lerInstancia(arquivo);
        g.setCapacidade(capacidade);

        auto inicio = chrono::high_resolution_clock::now();
        
        double custo = g.gulosoRandomizadoCMSTP(alpha, iteracoes);

        auto fim = chrono::high_resolution_clock::now();

        double tempo = chrono::duration<double>(fim-inicio).count();

        salvarResultadoCSV(arquivo, capacidade, "randomizado", alpha, iteracoes, -1, sementeExecucao, tempo, custo, g.getMediaSolucoes(), -1);
    }
}

void Experimentos::executarReativo(string arquivo, int capacidade, const vector<double> alphas, int iteracoes, int bloco)
{
    for(int i = 0; i < 10; i++){
        unsigned int sementeExecucao = semente + i;

        srand(sementeExecucao);

        Grafo g;

        g.lerInstancia(arquivo);
        g.setCapacidade(capacidade);

        auto inicio = chrono::high_resolution_clock::now();

        double custo = g.gulosoRandomizadoReativoCMSTP(
            alphas,
            iteracoes,
            bloco
        );

        auto fim = chrono::high_resolution_clock::now();

        double tempo = chrono::duration<double>(fim - inicio).count();
        salvarResultadoCSV(arquivo, capacidade, "reativo", -1, iteracoes, bloco, sementeExecucao, tempo, custo, g.getMediaSolucoes(), g.getMelhorAlphaReativo()
        );
    }
}

void Experimentos::executarInstancia(string arquivo, int capacidade)
{
    executarGuloso(arquivo, capacidade);

    for(double alpha : alphas)
    {
        executarRandomizado(
            arquivo,
            capacidade,
            alpha,
            iteracoesRandomizado
        );
    }

    executarReativo(
        arquivo,
        capacidade,
        alphas,
        iteracoesReativo,
        blocoReativo
    );
}

unsigned int Experimentos::gerarSemente()
{
    semente = static_cast<unsigned int>(time(nullptr));

    srand(semente);

    cout << "Semente de randomizacao: " << semente << endl;

    return semente;
}

void Experimentos::salvarResultadoCSV(
    string instancia,
    int capacidade,
    string algoritmo,
    double alpha,
    int iteracoes,
    int bloco,
    unsigned int semente,
    double tempo,
    double custo,
    double media,
    double melhorAlpha)
{
    string arquivoCSV = "resultados_experimento/resultados_experimento.csv";

    bool existe = fs::exists(arquivoCSV);

    ofstream csv(arquivoCSV, ios::app);

    if (!existe)
    {
        csv << "instancia;"
            << "capacidade;"
            << "algoritmo;"
            << "alpha;"
            << "iteracoes;"
            << "bloco;"
            << "semente;"
            << "tempo;"
            << "custo;"
            << "media;"
            << "melhor_alpha"
            << endl;
    }

    csv << fixed << setprecision(6);

    // Instância
    csv << fs::path(instancia).filename().string() << ";";

    // Capacidade
    csv << capacidade << ";";

    // Algoritmo
    csv << algoritmo << ";";

    // Alpha
    if (alpha >= 0)
        csv << alpha;
    csv << ";";

    // Iterações
    csv << iteracoes << ";";

    // Bloco
    if (bloco >= 0)
        csv << bloco;
    csv << ";";

    // Semente
    csv << semente << ";";

    // Tempo
    csv << tempo << ";";

    // Custo
    csv << custo << ";";

    // Média
    if (media >= 0)
        csv << media;
    csv << ";";

    // Melhor alpha
    if (melhorAlpha >= 0)
        csv << melhorAlpha;

    csv << endl;

    csv.close();
}