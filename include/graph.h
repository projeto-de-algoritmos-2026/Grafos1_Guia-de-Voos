#ifndef GRAPH_H
#define GRAPH_H

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x) * 1000)
#endif

typedef struct Aeroporto
{
    char nome[64];
    char iata[4];
    float latitude;
    float longitude;
} Aeroporto;

typedef struct No
{
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
    int numAeroportos;
    int numArestas;
    Aeroporto *aeroportos;
    Cabeca *cabecas;
} Grafo;

Grafo *criarGrafo();
void criarAeroporto(Grafo *grafo, Aeroporto aeroporto);
void criarVoo(Grafo *grafo, int origem, int destino, float peso);
void criarVooBidirecional(Grafo *grafo, int origem, int destino, float peso);
void imprimirGrafo(Grafo *grafo);

#endif