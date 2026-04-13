#include <stdio.h>
#include <stdlib.h>
#include "dfs.h"
#include "graph.h"

void dfsVisit(Grafo *grafo, int indice, Grafo *arvoreDFS, int *visitado)
{
    visitado[indice] = 1;
    No *aux = grafo->cabecas[indice].inicio;
    while (aux != NULL)
    {
        int indiceAdjacente = aux->indice;
        if (!visitado[indiceAdjacente])
        {
            criarVoo(arvoreDFS, indice, indiceAdjacente, 0);
            dfsVisit(grafo, indiceAdjacente, arvoreDFS, visitado);
        }
        aux = aux->prox;
    }
}

Grafo *dfs(Grafo *grafo)
{
    int *visitado = (int *)calloc(grafo->numAeroportos, sizeof(int));
    Grafo *arvoreDFS = criarGrafo();

    for (int i = 0; i < grafo->numAeroportos; i++)
        criarAeroporto(arvoreDFS, grafo->aeroportos[i]);

    for (int i = 0; i < grafo->numAeroportos; i++)
    {
        if (!visitado[i])
            dfsVisit(grafo, i, arvoreDFS, visitado);
    }

    return arvoreDFS;
}