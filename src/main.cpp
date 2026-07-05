#include "Grafo.hpp"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*Grafo lerInstancia(string caminhoArquivo, int capacidade) {
    ifstream arquivo(caminhoArquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir: " << caminhoArquivo << endl;
        exit(1);
    }
    string token;
    arquivo >> token;
    int nClientes;
    if (!isdigit(token[0])) {
        arquivo >> nClientes;
    } else {
        nClientes = stoi(token);
    }

    int N = nClientes + 1; 
    int deposito = nClientes; 

    Grafo g;
    g.criarGrafo(N);
    g.setDeposito(deposito);
    g.setCapacidade(capacidade);

    for (int i = 0; i < nClientes; i++) {
        g.setDemanda(i, 1);
    }
    g.setDemanda(deposito, 0); 

    double custo;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            arquivo >> custo;
            if (i < j && custo > 0 && custo < 900000) {
                g.inserirAresta(i, j, custo);
            }
        }
    }

    arquivo.close();
    return g;
}*/

/*int main() {
    // Semente de aleatoriedade exigida nas instruções (SÓ PODE CHAMAR UMA VEZ)
    srand(time(NULL));

    string caminho = "instancias/capmst1/tc40-1.txt"; 
    int capacidade = 5; 
    
    Grafo meuGrafo = lerInstancia(caminho, capacidade);
    cout << "Instancia carregada com sucesso!" << endl;

    double custoGuloso = meuGrafo.gulosoCMSTP();
    cout << "Custo final da rede (Guloso puro): " << custoGuloso << endl;

    // Testando o Randomizado com Alfa = 0.3 (30%) e rodando 30 vezes
    double custoRandomizado = meuGrafo.gulosoRandomizadoCMSTP(0.3, 30);
    cout << "Custo final da rede (Randomizado Alfa 0.3): " << custoRandomizado << endl;

    return 0;
}*/

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