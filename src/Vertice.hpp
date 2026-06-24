#ifndef VERTICE_HPP
#define VERTICE_HPP

class Vertice{
    private:
        int id;
        int demanda;
        bool ativo; //isso daqui é pra evitar que quando apague, ele não fique apontando para os vértices errados

    public:

    //construtor
    Vertice(int id, int demanda);

    int getId();
    int getDemanda();
    void setDemanda(int demanda);
    
    //funçoes pra saber se o vertice foi removido ou nn
    bool estaAtivo();
    void desativar();
};
#endif