#ifndef ALGORITMOS_HPP
#define ALGORITMOS_HPP

#include "Grafo.hpp"

using namespace std;

class Algoritmos
{
    public:
        double gulosoCMSTP(Grafo& grafo);

        double gulosoRandomizadoCMSTP(
            Grafo& grafo,
            double alpha,
            int numIteracoes
        );

        double gulosoRandomizadoReativoCMSTP(
            Grafo& grafo,
            const vector<double>& alphas,
            int numIteracoes,
            int tamanhoBloco
        );
};

#endif