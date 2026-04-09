#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x) * 1000)
#endif

#include <stdio.h>
#include <stdlib.h>

typedef struct Aeroporto
{
    char nome[64];
    char iata[4];
    float latitude;
    float longitude;
} Aeroporto;

// Lista de adjacências, o No representa uma "aresta" do grafo, ou seja, um voo entre dois aeroportos, o peso é a distância entre os aeroportos
typedef struct No
{
    // indice do aeroporto no vetor de aeroportos no grafo
    int indice;
    float peso;
    struct No *prox;
} No;

typedef struct Cabeca
{
    int tamanho;
    No *inicio;
    No *fim;
} Cabeca;

typedef struct Grafo
{
    int numAeroportos;     // número de aeroportos no grafo. É o número de vértices do grafo, também o número de listas de adjacências, número de cabeças, assim podendo simplificar o código
    int numArestas;        // número de arestas no grafo (corresponde ao número de voos entre os aeroportos, também ao número de nós nas listas de adjacências)
    Aeroporto *aeroportos; // vetor de aeroportos
    Cabeca *cabecas;       // vetor de cabeças para as listas de adjacências
} Grafo;

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