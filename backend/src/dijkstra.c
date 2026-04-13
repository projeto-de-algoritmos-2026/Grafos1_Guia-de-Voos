#include <stdio.h>
#include <stdlib.h>
#include "dijkstra.h"
#include "graph.h"
#include <float.h>
#include <limits.h>
#include "parser.h"

void subirHeap(Heap *heap, int indice)
{
    while (indice > 0)
    {
        int pai = (indice - 1) / 2;
        if (heap->nos[indice].distancia < heap->nos[pai].distancia)
        {
            NoHeap temp = heap->nos[indice];
            heap->nos[indice] = heap->nos[pai];
            heap->nos[pai] = temp;
            indice = pai;
        }
        else
        {
            break;
        }
    }
}

void descerHeap(Heap *heap, int indice)
{
    int menor = indice;
    int esquerda = 2 * indice + 1;
    int direita = 2 * indice + 2;

    if (esquerda < heap->tamanho && heap->nos[esquerda].distancia < heap->nos[menor].distancia)
        menor = esquerda;
    if (direita < heap->tamanho && heap->nos[direita].distancia < heap->nos[menor].distancia)
        menor = direita;

    if (menor != indice)
    {
        NoHeap temp = heap->nos[indice];
        heap->nos[indice] = heap->nos[menor];
        heap->nos[menor] = temp;
        descerHeap(heap, menor);
    }
}

Heap *criarHeap(int capacidade)
{
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    heap->nos = (NoHeap *)malloc(capacidade * sizeof(NoHeap));
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    return heap;
}

void inserirHeap(Heap *heap, int indice, float distancia)
{
    if (heap->tamanho == heap->capacidade)
        return;

    NoHeap novoNo = {indice, distancia};
    heap->nos[heap->tamanho] = novoNo;
    heap->tamanho++;
    subirHeap(heap, heap->tamanho - 1);
}

NoHeap removerMinimo(Heap *heap)
{
    NoHeap minimo = heap->nos[0];
    heap->nos[0] = heap->nos[heap->tamanho - 1];
    heap->tamanho--;
    descerHeap(heap, 0);
    return minimo;
}

int heapVazio(Heap *heap)
{
    return heap->tamanho == 0;
}

int *dijkstra(Grafo *grafo, const char *origemIata, const char *destinoIata, float *distanciaTotal)
{
    int origem = buscarHash(origemIata);
    int destino = buscarHash(destinoIata);

    if (origem == -1 || destino == -1)
    {
        printf("Aeroporto não encontrado\n");
        return NULL;
    }

    Heap *heap = criarHeap(grafo->numAeroportos);
    float *distancias = (float *)malloc(grafo->numAeroportos * sizeof(float));
    int *anterior = (int *)malloc(grafo->numAeroportos * sizeof(int));

    for (int i = 0; i < grafo->numAeroportos; i++)
    {
        distancias[i] = FLT_MAX;
        anterior[i] = -1;
    }

    distancias[origem] = 0;
    inserirHeap(heap, origem, 0);

    while (!heapVazio(heap))
    {
        NoHeap atual = removerMinimo(heap);
        if (atual.indice == destino)
            break;

        No *aux = grafo->cabecas[atual.indice].inicio;
        while (aux != NULL)
        {
            float novaDistancia = distancias[atual.indice] + aux->peso;
            if (novaDistancia < distancias[aux->indice])
            {
                distancias[aux->indice] = novaDistancia;
                anterior[aux->indice] = atual.indice;
                inserirHeap(heap, aux->indice, novaDistancia);
            }
            aux = aux->prox;
        }
    }

    free(heap->nos);
    free(heap);
    free(distancias);
    *distanciaTotal = distancias[destino];
    return anterior;
}
