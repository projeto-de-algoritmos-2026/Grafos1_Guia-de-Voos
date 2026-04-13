#include <stdio.h>
#include <stdlib.h>
#include "bfs.h"
#include "graph.h"
#include "parser.h"

Fila *criarFila()
{
    Fila *fila = (Fila *)malloc(sizeof(Fila));
    fila->inicio = NULL;
    fila->fim = NULL;
    return fila;
}

void insereFila(Fila *fila, int indice)
{
    NoFila *novo = (NoFila *)malloc(sizeof(NoFila));
    novo->indice = indice;
    novo->prox = NULL;

    if (fila->inicio == NULL)
    {
        fila->inicio = novo;
        fila->fim = novo;
    }
    else
    {
        fila->fim->prox = novo;
        fila->fim = novo;
    }
}

int removeFila(Fila *fila)
{
    NoFila *limpar = fila->inicio;
    int removido = limpar->indice;
    fila->inicio = limpar->prox;
    free(limpar);
    return removido;
}

int filaVazia(Fila *fila)
{
    return fila->inicio == NULL;
}

Grafo *bfs(Grafo *grafo)
{
    Fila *fila = criarFila();
    int *visitado = (int *)calloc(grafo->numAeroportos, sizeof(int));
    Grafo *arvoreBFS = criarGrafo();

    // cria os vertices da bfs tree
    for (int i = 0; i < grafo->numAeroportos; i++)
    {
        criarAeroporto(arvoreBFS, grafo->aeroportos[i]);
    }

    for (int i = 0; i < grafo->numAeroportos; i++)
    {
        if (!visitado[i])
        {

            insereFila(fila, i);
            visitado[i] = 1;
            while (!filaVazia(fila))
            {
                int indiceObservado = removeFila(fila);
                No *auxiliar = grafo->cabecas[indiceObservado].inicio;
                while (auxiliar != NULL)
                {
                    if (!visitado[auxiliar->indice])
                    {
                        insereFila(fila, auxiliar->indice);
                        visitado[auxiliar->indice] = 1;
                        criarVoo(arvoreBFS, indiceObservado, auxiliar->indice, 0);
                    }
                    auxiliar = auxiliar->prox;
                }
            }
        }
    }

    free(visitado);
    return arvoreBFS;
}

int *bfsMenorCaminho(Grafo *grafo, const char *origemIata, const char *destinoIata)
{
    int origem = buscarHash(origemIata);
    int destino = buscarHash(destinoIata);

    if (origem == -1 || destino == -1)
    {
        printf("Aeroporto não encontrado\n");
        return NULL;
    }
    Fila *fila = criarFila();
    // uso calloc porque inicializa o vetor com 0, mas 0 é um indice válido
    int *visitado = (int *)calloc(grafo->numAeroportos, sizeof(int));
    int *anterior = (int *)calloc(grafo->numAeroportos, sizeof(int));

    for (int i = 0; i < grafo->numAeroportos; i++)
        anterior[i] = -1;

    insereFila(fila, origem);
    visitado[origem] = 1;
    while (!filaVazia(fila))
    {
        int indiceObservado = removeFila(fila);
        if (indiceObservado == destino)
            break;
        No *aux = grafo->cabecas[indiceObservado].inicio;
        while (aux != NULL)
        {
            if (!visitado[aux->indice])
            {
                insereFila(fila, aux->indice);
                visitado[aux->indice] = 1;
                anterior[aux->indice] = indiceObservado;
            }
            aux = aux->prox;
        }
    }

    return anterior;
}

int *reconstruirCaminho(Grafo *grafo, int *anterior, const char *destinoIata, int *tamanho)
{
    int destino = buscarHash(destinoIata);
    int *caminho = (int *)malloc(6074 * sizeof(int));
    *tamanho = 0;
    int atual = destino;

    while (atual != -1)
    {
        caminho[(*tamanho)++] = atual;
        atual = anterior[atual];
    }

    return caminho;
}