#include "LeitorInstancia.hpp"

#include <fstream>
#include <iostream>

using namespace std;

bool LeitorInstancia::lerInstancia(Grafo& grafo, string nomeArquivo)
{
    ifstream arquivo(nomeArquivo);

    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo!" << endl;
        return false;
    }

    int totalVertices;
    arquivo >> totalVertices;

    grafo.criarGrafo(totalVertices);

    // O depósito é o último vértice
    grafo.setDeposito(totalVertices - 1);

    // Nas instâncias tc e te todos os clientes têm demanda 1
    for (int i = 0; i < totalVertices; i++)
    {
        grafo.setDemanda(i, 1);
    }

    // Não precisa saber quem é o deposito
    grafo.setDemanda(totalVertices - 1, 0);

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
            int peso;
            arquivoDemandas >> peso;
            grafo.setDemanda(i, peso);
        }

        grafo.setDemanda(totalVertices - 1, 0);
        arquivoDemandas.close();
    }

    // Lê a matriz de custos
    for (int i = 0; i < totalVertices; i++)
    {
        for (int j = 0; j < totalVertices; j++)
        {
            double custo;
            arquivo >> custo;

            // Na diagonal, 0 ou 1000 significa sem aresta (ignora)
            if (i == j) custo = 0;

            grafo.inserirAresta(i, j, custo);
        }
    }

    arquivo.close();

    return true;
} 