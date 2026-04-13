#include <stdio.h>
#include <stdlib.h>
#include "kosaraju.h"
#include "graph.h"
#include <string.h>

Pilha *criarPilha()
{
    Pilha *pilha = (Pilha *)malloc(sizeof(Pilha));
    pilha->topo = NULL;
    return pilha;
}

void empilhar(Pilha *pilha, int indice)
{
    NoPilha *novo = (NoPilha *)malloc(sizeof(NoPilha));
    novo->indice = indice;
    novo->prox = pilha->topo;
    pilha->topo = novo;
}

int desempilhar(Pilha *pilha)
{
    NoPilha *limpar = pilha->topo;
    int removido = limpar->indice;
    pilha->topo = limpar->prox;
    free(limpar);
    return removido;
}

int pilhaVazia(Pilha *pilha)
{
    return pilha->topo == NULL;
}

Grafo *inverterGrafo(Grafo *grafo)
{
    Grafo *invertido = criarGrafo();
    for (int i = 0; i < grafo->numAeroportos; i++)
        criarAeroporto(invertido, grafo->aeroportos[i]);
    for (int i = 0; i < grafo->numAeroportos; i++)
    {
        No *aux = grafo->cabecas[i].inicio;
        while (aux != NULL)
        {
            criarVoo(invertido, aux->indice, i, aux->peso);
            aux = aux->prox;
        }
    }
    return invertido;
}

void dfsKosaraju(Grafo *grafo, int indice, int *visitado, Pilha *pilha)
{
    Pilha *stack = criarPilha();
    empilhar(stack, indice);

    while (!pilhaVazia(stack))
    {
        int atual = stack->topo->indice;
        if (!visitado[atual])
        {
            visitado[atual] = 1;
            No *aux = grafo->cabecas[atual].inicio;
            int empilhouFilho = 0;
            while (aux != NULL)
            {
                if (!visitado[aux->indice])
                {
                    empilhar(stack, aux->indice);
                    empilhouFilho = 1;
                    break;
                }
                aux = aux->prox;
            }
            if (!empilhouFilho)
            {
                desempilhar(stack);
                empilhar(pilha, atual);
            }
        }
        else
        {
            desempilhar(stack);
        }
    }
}

void dfsComponente(Grafo *grafo, int indice, int *visitado, int *scc, int componente)
{
    Pilha *stack = criarPilha();
    empilhar(stack, indice);

    while (!pilhaVazia(stack))
    {
        int atual = desempilhar(stack);
        if (visitado[atual])
            continue;
        visitado[atual] = 1;
        scc[atual] = componente;
        No *aux = grafo->cabecas[atual].inicio;
        while (aux != NULL)
        {
            if (!visitado[aux->indice])
                empilhar(stack, aux->indice);
            aux = aux->prox;
        }
    }
}

int *kosaraju(Grafo *grafo)
{
    Pilha *pilha = criarPilha();
    int *visitado = (int *)calloc(grafo->numAeroportos, sizeof(int));

    for (int i = 0; i < grafo->numAeroportos; i++)
        if (!visitado[i])
            dfsKosaraju(grafo, i, visitado, pilha);

    Grafo *invertido = inverterGrafo(grafo);

    int *scc = (int *)malloc(grafo->numAeroportos * sizeof(int));
    memset(visitado, 0, grafo->numAeroportos * sizeof(int));

    int componente = 0;
    while (!pilhaVazia(pilha))
    {
        int v = desempilhar(pilha);
        if (!visitado[v])
        {
            dfsComponente(invertido, v, visitado, scc, componente);
            componente++;
        }
    }

    free(visitado);
    return scc;
}