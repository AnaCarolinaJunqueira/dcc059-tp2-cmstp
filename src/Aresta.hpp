#ifndef ARESTA_HPP
#define ARESTA_HPP

class Aresta {
    private:
        int origem;
        int destino;
        double custo;

    public:
        //construtor        
        Aresta(int origem, int destino, double custo);

        int getOrigem();
        int getDestino();
        double getCusto();

};

#endif