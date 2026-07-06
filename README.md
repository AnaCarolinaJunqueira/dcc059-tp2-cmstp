# Trabalho 2 - DCC059 - Teoria dos Grafos

Implementação em C++ para o Problema da Árvore Geradora Capacitada de Custo Mínimo  
(Capacitated Minimum Spanning Tree Problem - CMSTP).

## Descrição

O CMSTP consiste em encontrar uma árvore geradora de custo mínimo enraizada em um depósito, respeitando uma restrição de capacidade. Ao remover a raiz da árvore, cada subárvore resultante deve possuir demanda total menor ou igual à capacidade Q.

## Algoritmos implementados

- Algoritmo guloso;
- Algoritmo guloso randomizado;
- Algoritmo guloso randomizado reativo.

## Instâncias

As instâncias utilizadas foram obtidas da OR-Library e separadas em arquivos individuais para facilitar a leitura pelo programa.

O projeto possui suporte para:

- instâncias `capmst1` com demandas unitárias;
- instâncias `capmst2` com demandas não unitárias, usando arquivos priz para demandas.

## Compilação e execução

As intruções detalhadas de execução se encontram no arquivo:

- COMO_EXECUTAR.txt