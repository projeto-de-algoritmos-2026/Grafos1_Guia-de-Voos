#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "graph.h"
#include "parser.h"
#include "bfs.h"
#include "dfs.h"
#include "dijkstra.h"
#include "kosaraju.h"
#include "server.h"

Grafo *grafo;

void limparTela()
{
    system("clear");
}

void pausar()
{
    printf("\nPressione Enter para continuar...");
    getchar();
    getchar();
}

void toUpperStr(char *str)
{
    for (int i = 0; str[i]; i++)
        str[i] = toupper(str[i]);
}

void lerIata(const char *label, char *dest)
{
    printf("%s", label);
    scanf("%3s", dest);
    toUpperStr(dest);
}

void imprimirCaminho(int *caminho, int tamanho)
{
    for (int i = tamanho - 1; i >= 0; i--)
    {
        printf("%s", grafo->aeroportos[caminho[i]].iata);
        if (i > 0)
            printf(" → ");
    }
    printf("\n");
}

void imprimirSeparador()
{
    printf("  ──────────────────────────────────────────\n");
}

int validarCaminho(int *anterior, const char *destinoIata, const char *origemIata)
{
    int destino = buscarHash(destinoIata);
    int origem = buscarHash(origemIata);
    if (destino == -1 || origem == -1)
    {
        printf("  Aeroporto não encontrado.\n");
        return 0;
    }
    if (anterior[destino] == -1 && destino != origem)
    {
        printf("  Caminho não encontrado entre %s e %s.\n", origemIata, destinoIata);
        return 0;
    }
    return 1;
}

void menuMenorEscalas()
{
    limparTela();
    char origem[4], destino[4];
    imprimirSeparador();
    printf("  MENOR NÚMERO DE ESCALAS (BFS)\n");
    imprimirSeparador();
    lerIata("  Origem  (IATA): ", origem);
    lerIata("  Destino (IATA): ", destino);

    int *ant = bfsMenorCaminho(grafo, origem, destino);
    if (!ant)
    {
        pausar();
        return;
    }
    if (!validarCaminho(ant, destino, origem))
    {
        free(ant);
        pausar();
        return;
    }

    int tam = 0;
    int *cam = reconstruirCaminho(grafo, ant, destino, &tam);
    float dist = calcularDistanciaCaminho(grafo, cam, tam);

    imprimirSeparador();
    printf("  Rota:      ");
    imprimirCaminho(cam, tam);
    printf("  Escalas:   %d\n", tam - 1);
    printf("  Distância: %.0f km\n", dist);
    imprimirSeparador();

    free(ant);
    free(cam);
    pausar();
}

void menuMenorDistancia()
{
    limparTela();
    char origem[4], destino[4];
    imprimirSeparador();
    printf("  MENOR DISTÂNCIA (DIJKSTRA)\n");
    imprimirSeparador();
    lerIata("  Origem  (IATA): ", origem);
    lerIata("  Destino (IATA): ", destino);

    float distancia = 0;
    int *ant = dijkstra(grafo, origem, destino, &distancia);
    if (!ant)
    {
        pausar();
        return;
    }
    if (!validarCaminho(ant, destino, origem))
    {
        free(ant);
        pausar();
        return;
    }

    int tam = 0;
    int *cam = reconstruirCaminho(grafo, ant, destino, &tam);

    imprimirSeparador();
    printf("  Rota:      ");
    imprimirCaminho(cam, tam);
    printf("  Escalas:   %d\n", tam - 1);
    printf("  Distância: %.0f km\n", distancia);
    imprimirSeparador();

    free(ant);
    free(cam);
    pausar();
}

void menuComparativo()
{
    limparTela();
    char origem[4], destino[4];
    imprimirSeparador();
    printf("  COMPARATIVO BFS vs DIJKSTRA\n");
    imprimirSeparador();
    lerIata("  Origem  (IATA): ", origem);
    lerIata("  Destino (IATA): ", destino);

    int *antBFS = bfsMenorCaminho(grafo, origem, destino);
    if (!antBFS)
    {
        pausar();
        return;
    }
    if (!validarCaminho(antBFS, destino, origem))
    {
        free(antBFS);
        pausar();
        return;
    }
    int tamBFS = 0;
    int *camBFS = reconstruirCaminho(grafo, antBFS, destino, &tamBFS);
    float distBFS = calcularDistanciaCaminho(grafo, camBFS, tamBFS);

    float distDijk = 0;
    int *antDijk = dijkstra(grafo, origem, destino, &distDijk);
    if (!antDijk)
    {
        free(antBFS);
        free(camBFS);
        pausar();
        return;
    }
    int tamDijk = 0;
    int *camDijk = reconstruirCaminho(grafo, antDijk, destino, &tamDijk);

    imprimirSeparador();
    printf("  BFS (menos escalas)\n");
    printf("  Rota:      ");
    imprimirCaminho(camBFS, tamBFS);
    printf("  Escalas:   %d | Distância: %.0f km\n", tamBFS - 1, distBFS);

    imprimirSeparador();
    printf("  DIJKSTRA (menor distância)\n");
    printf("  Rota:      ");
    imprimirCaminho(camDijk, tamDijk);
    printf("  Escalas:   %d | Distância: %.0f km\n", tamDijk - 1, distDijk);

    imprimirSeparador();
    if (distBFS - distDijk > 0)
        printf("  Dijkstra economizou %.0f km\n", distBFS - distDijk);
    else
        printf("  Mesma distância pelos dois caminhos\n");
    if (tamDijk - tamBFS > 0)
        printf("  BFS economizou %d escala(s)\n", tamDijk - tamBFS);
    else
        printf("  Mesmo número de escalas pelos dois caminhos\n");
    imprimirSeparador();

    free(antBFS);
    free(camBFS);
    free(antDijk);
    free(camDijk);
    pausar();
}

void menuKosaraju()
{
    limparTela();
    imprimirSeparador();
    printf("  COMPONENTES FORTEMENTE CONECTADOS (KOSARAJU)\n");
    imprimirSeparador();
    printf("  Rodando...\n\n");

    int *scc = kosaraju(grafo);

    int maxComp = 0;
    for (int i = 0; i < grafo->numAeroportos; i++)
        if (scc[i] > maxComp)
            maxComp = scc[i];

    int *tamanhos = (int *)calloc(maxComp + 1, sizeof(int));
    for (int i = 0; i < grafo->numAeroportos; i++)
        tamanhos[scc[i]]++;

    int maior = 0;
    int maioresQue1 = 0;
    int isolados = 0;
    for (int i = 0; i <= maxComp; i++)
    {
        if (tamanhos[i] > maior)
            maior = tamanhos[i];
        if (tamanhos[i] > 1)
            maioresQue1++;
        if (tamanhos[i] == 1)
            isolados++;
    }

    printf("  Total de SCCs:               %d\n", maxComp + 1);
    printf("  Maior componente:            %d aeroportos\n", maior);
    printf("  SCCs com 2+ aeroportos:      %d\n", maioresQue1);
    printf("  Aeroportos isolados (SCC=1): %d\n", isolados);
    imprimirSeparador();

    free(scc);
    free(tamanhos);
    pausar();
}

void menuInfo()
{
    limparTela();
    imprimirSeparador();
    printf("  INFORMAÇÕES DO GRAFO\n");
    imprimirSeparador();
    printf("  Aeroportos carregados: %d\n", grafo->numAeroportos);
    printf("  Rotas carregadas:      %d\n", grafo->numArestas);

    int isolados = 0;
    for (int i = 0; i < grafo->numAeroportos; i++)
        if (grafo->cabecas[i].tamanho == 0)
            isolados++;

    printf("  Aeroportos sem rotas:  %d\n", isolados);
    printf("  Aeroportos com rotas:  %d\n", grafo->numAeroportos - isolados);
    imprimirSeparador();
    pausar();
}

int main()
{
    limparTela();
    printf("\n  Carregando dados...\n");
    grafo = criarGrafo();
    carregarAeroportos(grafo, "data/airports.dat");
    carregarRotas(grafo, "data/routes.dat");
    printf("  Carregados: %d aeroportos, %d rotas.\n", grafo->numAeroportos, grafo->numArestas);

    struct MHD_Daemon *daemon = iniciarServidor(grafo, 8080);
    if (!daemon)
    {
        printf("  Erro ao iniciar servidor\n");
        return 1;
    }
    printf("  Servidor rodando em http://localhost:8080\n");
    printf("  Pressione Enter para encerrar...\n");
    getchar();
    MHD_stop_daemon(daemon);

    int opcao;
    do
    {
        limparTela();
        printf("\n");
        printf("  ╔══════════════════════════════════════╗\n");
        printf("  ║       SISTEMA DE ROTAS AÉREAS        ║\n");
        printf("  ║                                      ║\n");
        printf("  ╠══════════════════════════════════════╣\n");
        printf("  ║  1.  Menor número de escalas (BFS)   ║\n");
        printf("  ║  2.  Menor distância (Dijkstra)      ║\n");
        printf("  ║  3.  Comparativo BFS vs Dijkstra     ║\n");
        printf("  ║  4.  Componentes SCCs (Kosaraju)     ║\n");
        printf("  ║  5.  Informações do grafo            ║\n");
        printf("  ║  0.  Sair                            ║\n");
        printf("  ╚══════════════════════════════════════╝\n");
        printf("\n  Opção: ");
        scanf("%d", &opcao);
        printf("\n");

        switch (opcao)
        {
        case 1:
            menuMenorEscalas();
            break;
        case 2:
            menuMenorDistancia();
            break;
        case 3:
            menuComparativo();
            break;
        case 4:
            menuKosaraju();
            break;
        case 5:
            menuInfo();
            break;
        case 0:
            limparTela();
            printf("  Saindo...\n\n");
            break;
        default:
            printf("  Opção inválida.\n");
            pausar();
        }
    } while (opcao != 0);

    return 0;
}