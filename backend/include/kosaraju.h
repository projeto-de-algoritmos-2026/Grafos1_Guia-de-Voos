#ifndef KOSARAJU_H
#define KOSARAJU_H
#include "graph.h"

// estrutura de pilha pra não estourar a head com pilha de recursão
typedef struct NoPilha
{
    int indice;
    struct NoPilha *prox;
} NoPilha;

typedef struct Pilha
{
    NoPilha *topo;
} Pilha;

Pilha *criarPilha();
void empilhar(Pilha *pilha, int indice);
int desempilhar(Pilha *pilha);
int pilhaVazia(Pilha *pilha);
Grafo *inverterGrafo(Grafo *grafo);
void dfsKosaraju(Grafo *grafo, int indice, int *visitado, Pilha *pilha);
void dfsComponente(Grafo *grafo, int indice, int *visitado, int *scc, int componente);
int *kosaraju(Grafo *grafo);

#endif