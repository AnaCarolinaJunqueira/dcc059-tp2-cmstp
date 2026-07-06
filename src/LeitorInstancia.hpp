#ifndef LEITOR_INSTANCIA_HPP
#define LEITOR_INSTANCIA_HPP

#include <string>
#include "Grafo.hpp"

using namespace std;

class LeitorInstancia
{
    public:
        
        bool lerInstancia(Grafo& grafo, string nomeArquivo);
};

#endif