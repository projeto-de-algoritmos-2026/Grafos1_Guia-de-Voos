#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "parser.h"

int main()
{
    Grafo *grafo = criarGrafo();
    carregarAeroportos(grafo, "data/airports.dat");
    carregarRotas(grafo, "data/routes.dat");
    printf("Aeroportos carregados: %d\n", grafo->numAeroportos);
    printf("Arestas carregadas: %d\n", grafo->numArestas);
    return 0;
}

// TODO - Implementar o parser para ler os dados dos aeroportos e rotas a partir do .dat
// TODO - Implementar algoritmos de grafos para encontrar rotas, calcular distâncias, etc.
