#ifndef DFS_H
#define DFS_H

#include "graph.h"

void dfsVisit(Grafo *grafo, int indice, Grafo *arvoreDFS, int *visitado);
Grafo *dfs(Grafo *grafo);

#endif