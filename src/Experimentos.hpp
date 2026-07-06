#ifndef EXPERIMENTOS_HPP
#define EXPERIMENTOS_HPP

#include <string>
#include <vector>

using namespace std;

class Experimentos
{
    public:
        Experimentos();
        void executarTodos();

    private:

        vector<double> alphas = {0.05, 0.15, 0.30};

        const int iteracoesRandomizado = 30;
        const int iteracoesReativo = 300;
        const int blocoReativo = 30;

        void executarCapmst1();
        void executarCapmst2();

        void executarInstancia(string arquivo, int capacidade);

        void executarGuloso(string arquivo, int capacidade);
        void executarRandomizado(string arquivo, int capacidade, double alpha, int iteracoes);
        void executarReativo(string arquivo, int capacidade, const vector<double> alphas, int iteracoes, int bloco);

        void salvarResultadoCSV(string instancia, int capacidade, string algoritmo, double alpha, int iteracoes, int bloco, unsigned int semente, double tempo, double custo, double media = -1, double melhorAlpha = -1);

        unsigned int gerarSemente();
        unsigned int semente;
};


#endif