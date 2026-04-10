#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define RAIO_TERRA 6371.0

Grafo *criarGrafo()
{
    Grafo *grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->numAeroportos = 0;
    grafo->numArestas = 0;
    grafo->aeroportos = NULL;
    grafo->cabecas = NULL;
    return grafo;
}

void criarAeroporto(Grafo *grafo, Aeroporto aeroporto)
{
    grafo->aeroportos = (Aeroporto *)realloc(grafo->aeroportos, (grafo->numAeroportos + 1) * sizeof(Aeroporto));
    grafo->aeroportos[grafo->numAeroportos] = aeroporto;
    grafo->cabecas = (Cabeca *)realloc(grafo->cabecas, (grafo->numAeroportos + 1) * sizeof(Cabeca));
    grafo->cabecas[grafo->numAeroportos].tamanho = 0;
    grafo->cabecas[grafo->numAeroportos].inicio = NULL;
    grafo->cabecas[grafo->numAeroportos].fim = NULL;
    grafo->numAeroportos++;
}

void criarVoo(Grafo *grafo, int origem, int destino, float peso)
{
    No *novoVoo = (No *)malloc(sizeof(No));
    novoVoo->indice = destino;
    novoVoo->peso = peso;
    novoVoo->prox = NULL;

    if (grafo->cabecas[origem].inicio == NULL)
    {
        grafo->cabecas[origem].inicio = novoVoo;
        grafo->cabecas[origem].fim = novoVoo;
    }
    else
    {
        grafo->cabecas[origem].fim->prox = novoVoo;
        grafo->cabecas[origem].fim = novoVoo;
    }
    grafo->cabecas[origem].tamanho++;
    grafo->numArestas++;
}

void criarVooBidirecional(Grafo *grafo, int origem, int destino, float peso)
{
    criarVoo(grafo, origem, destino, peso);
    criarVoo(grafo, destino, origem, peso);
}

void imprimirGrafo(Grafo *grafo)
{
    for (int i = 0; i < grafo->numAeroportos; i++)
    {
        printf("Aeroporto: %s (%s)\n", grafo->aeroportos[i].nome, grafo->aeroportos[i].iata);
        No *atual = grafo->cabecas[i].inicio;
        while (atual != NULL)
        {
            printf("  Voo para: %s (%s) - Distância: %.2f km\n", grafo->aeroportos[atual->indice].nome, grafo->aeroportos[atual->indice].iata, atual->peso);
            atual = atual->prox;
        }
    }
}

float haversine(float lat1, float lon1, float lat2, float lon2)
{
    float dlat = (lat2 - lat1) * M_PI / 180.0;
    float dlon = (lon2 - lon1) * M_PI / 180.0;
    float a = sin(dlat / 2) * sin(dlat / 2) +
              cos(lat1 * M_PI / 180.0) * cos(lat2 * M_PI / 180.0) *
                  sin(dlon / 2) * sin(dlon / 2);
    return RAIO_TERRA * 2 * atan2(sqrt(a), sqrt(1 - a));
}