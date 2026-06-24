#ifndef ARESTA_HPP
#define ARESTA_HPP

class Aresta {
    private:
        int destino;
        double custo;

    public:
        //construtor        
        Aresta(int destino, double custo);
        
        int getDestino();
        double getCusto();

};

#endif