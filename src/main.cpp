#include "Grafo.hpp"
#include <iostream>

using namespace std;

int main()
{
    Grafo g;

    if (!g.lerInstancia("instancias/capmst1/tc40-1.txt"))
    {
        cout << "Erro ao ler a instância!" << endl;
        return 1;
    }

    g.setCapacidade(3);

    cout << "Instância carregada com sucesso!" << endl;

    if (!g.lerInstancia("instancias/capmst2/cm50r1.dat"))
    {
        cout << "Erro ao ler a instância!" << endl;
        return 1;
    }

    g.setCapacidade(200);

    cout << "Instância carregada com sucesso!" << endl;

    return 0;
}