#ifndef BFS_H
#define BFS_H
#endif

#include "graph.h"

typedef struct NoFila
{
    int indice;
    struct NoFila *prox;
} NoFila;

typedef struct Fila
{
    NoFila *inicio;
    NoFila *fim;
} Fila;

Fila *criarFila();
void insereFila(Fila *fila, int indice);
int removeFila(Fila *fila);
int filaVazia(Fila *fila);
Grafo *bfs(Grafo *grafo);
int *bfsMenorCaminho(Grafo *grafo, const char *origemIata, const char *destinoIata);
int *reconstruirCaminho(Grafo *grafo, int *anterior, const char *destinoIata, int *tamanho);