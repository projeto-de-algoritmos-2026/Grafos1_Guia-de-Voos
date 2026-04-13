#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "graph.h"

typedef struct NoHeap
{
    int indice;
    float distancia;
} NoHeap;

typedef struct Heap
{
    NoHeap *nos;
    int tamanho;
    int capacidade;
} Heap;

Heap *criarHeap(int capacidade);
void inserirHeap(Heap *heap, int indice, float distancia);
NoHeap removerMinimo(Heap *heap);
int heapVazio(Heap *heap);
void subirHeap(Heap *heap, int indice);
void descerHeap(Heap *heap, int indice);
int *dijkstra(Grafo *grafo, const char *origemIata, const char *destinoIata, float *distanciaTotal);

#endif