#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

int main()
{
    Grafo *grafo = criarGrafo();

    Aeroporto bsb = {"Aeroporto de Brasília", "BSB", -15.8711, -47.9186};
    Aeroporto gru = {"Aeroporto de Guarulhos", "GRU", -23.4356, -46.4731};
    Aeroporto gig = {"Aeroporto do Galeão", "GIG", -22.8099, -43.2505};

    criarAeroporto(grafo, bsb);
    criarAeroporto(grafo, gru);
    criarAeroporto(grafo, gig);

    criarVooBidirecional(grafo, 0, 1, 873.0);
    criarVooBidirecional(grafo, 0, 2, 1160.0);
    criarVooBidirecional(grafo, 1, 2, 357.0);

    imprimirGrafo(grafo);

    return 0;
}

//TODO - Implementar o parser para ler os dados dos aeroportos e rotas a partir do .dat
//TODO - Implementar algoritmos de grafos para encontrar rotas, calcular distâncias, etc.
