#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "parser.h"
#include "bfs.h"
#include "dfs.h"

int main()
{
    // teste do parser
    Grafo *grafo = criarGrafo();
    carregarAeroportos(grafo, "data/airports.dat");
    carregarRotas(grafo, "data/routes.dat");
    printf("Aeroportos carregados: %d\n", grafo->numAeroportos);
    printf("Arestas carregadas: %d\n", grafo->numArestas);

    // teste da bfs
    Grafo *arvore = bfs(grafo);
    printf("Grafo original: %d aeroportos, %d arestas\n", grafo->numAeroportos, grafo->numArestas);
    printf("Arvore BFS:     %d aeroportos, %d arestas\n", arvore->numAeroportos, arvore->numArestas);

    // teste da dfs
    Grafo *arvoreDFS = dfs(grafo);
    printf("Arvore DFS: %d aeroportos, %d arestas\n", arvoreDFS->numAeroportos, arvoreDFS->numArestas);

    // prova real da bfs
    int isolados = 0;
    for (int i = 0; i < grafo->numAeroportos; i++)
        if (grafo->cabecas[i].tamanho == 0)
            isolados++;
    printf("Aeroportos sem rotas: %d\n", isolados);

    // testes bfs menor caminho (menor quantidade de escalas)
    int *ant = bfsMenorCaminho(grafo, "BSB", "YPO");
    int tam = 0;
    int *cam = reconstruirCaminho(grafo, ant, "YPO", &tam);
    printf("Caminho BSB -> YPO (%d escalas):\n", tam - 1);
    for (int i = tam - 1; i >= 0; i--)
        printf("%s ", grafo->aeroportos[cam[i]].iata);
    printf("\n");

    // roda a bfs 6074 vezes pra encontrar o mais isolado saindo de brasilia
    /*
    int maxEscalas = 0;
    char *maxIata = NULL;


    for (int i = 0; i < grafo->numAeroportos; i++)
    {
        int *ant = bfsMenorCaminho(grafo, "BSB", grafo->aeroportos[i].iata);
        int tam = 0;
        int *cam = reconstruirCaminho(grafo, ant, grafo->aeroportos[i].iata, &tam);
        if (tam > maxEscalas)
        {
            maxEscalas = tam;
            maxIata = grafo->aeroportos[i].iata;
        }
        free(ant);
        free(cam);
    }

    printf("Aeroporto mais distante: %s com %d escalas\n", maxIata, maxEscalas - 1);
    */

    return 0;
}

// TODO - Implementar algoritmos de grafos para encontrar rotas, calcular distâncias, etc.
