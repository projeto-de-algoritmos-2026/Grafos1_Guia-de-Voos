#include "server.h"
#include "graph.h"
#include "parser.h"
#include "bfs.h"
#include "dijkstra.h"
#include "kosaraju.h"
#include <microhttpd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static Grafo *grafoGlobal;

void toUpperStrServer(char *str)
{
    for (int i = 0; str[i]; i++)
        str[i] = toupper(str[i]);
}

const char *getParam(struct MHD_Connection *connection, const char *chave)
{
    return MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, chave);
}

enum MHD_Result responderJSON(struct MHD_Connection *connection, const char *json, int status)
{
    struct MHD_Response *response = MHD_create_response_from_buffer(
        strlen(json), (void *)json, MHD_RESPMEM_MUST_COPY);
    MHD_add_response_header(response, "Content-Type", "application/json");
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
    enum MHD_Result ret = MHD_queue_response(connection, status, response);
    MHD_destroy_response(response);
    return ret;
}

void montarCaminhoJSON(Grafo *grafo, int *cam, int tam, char *out, int outSize)
{
    strcpy(out, "[");
    for (int i = tam - 1; i >= 0; i--)
    {
        char entry[256];
        snprintf(entry, sizeof(entry),
                 "{\"iata\":\"%s\",\"nome\":\"%s\",\"lat\":%.4f,\"lon\":%.4f}",
                 grafo->aeroportos[cam[i]].iata,
                 grafo->aeroportos[cam[i]].nome,
                 grafo->aeroportos[cam[i]].latitude,
                 grafo->aeroportos[cam[i]].longitude);
        strcat(out, entry);
        if (i > 0)
            strcat(out, ",");
    }
    strcat(out, "]");
}

enum MHD_Result rotaInfo(struct MHD_Connection *connection)
{
    int isolados = 0;
    for (int i = 0; i < grafoGlobal->numAeroportos; i++)
        if (grafoGlobal->cabecas[i].tamanho == 0)
            isolados++;

    char json[256];
    snprintf(json, sizeof(json),
             "{\"aeroportos\":%d,\"rotas\":%d,\"semRotas\":%d,\"comRotas\":%d}",
             grafoGlobal->numAeroportos,
             grafoGlobal->numArestas,
             isolados,
             grafoGlobal->numAeroportos - isolados);
    return responderJSON(connection, json, MHD_HTTP_OK);
}

enum MHD_Result rotaBFS(struct MHD_Connection *connection)
{
    const char *origem = getParam(connection, "origem");
    const char *destino = getParam(connection, "destino");
    if (!origem || !destino)
        return responderJSON(connection, "{\"erro\":\"origem e destino obrigatorios\"}", MHD_HTTP_BAD_REQUEST);

    char org[4], dst[4];
    strncpy(org, origem, sizeof(org));
    strncpy(dst, destino, sizeof(dst));
    toUpperStrServer(org);
    toUpperStrServer(dst);

    int *ant = bfsMenorCaminho(grafoGlobal, org, dst);
    if (!ant)
        return responderJSON(connection, "{\"erro\":\"aeroporto nao encontrado\"}", MHD_HTTP_NOT_FOUND);

    int destIdx = buscarHash(dst);
    if (ant[destIdx] == -1 && buscarHash(org) != destIdx)
    {
        free(ant);
        return responderJSON(connection, "{\"erro\":\"caminho nao encontrado\"}", MHD_HTTP_NOT_FOUND);
    }

    int tam = 0;
    int *cam = reconstruirCaminho(grafoGlobal, ant, dst, &tam);
    float dist = calcularDistanciaCaminho(grafoGlobal, cam, tam);

    char *camStr = (char *)malloc(32768);
    montarCaminhoJSON(grafoGlobal, cam, tam, camStr, 32768);

    char *json = (char *)malloc(40000);
    snprintf(json, 40000,
             "{\"caminho\":%s,\"escalas\":%d,\"distancia\":%.0f}",
             camStr, tam - 1, dist);

    enum MHD_Result ret = responderJSON(connection, json, MHD_HTTP_OK);
    free(ant);
    free(cam);
    free(camStr);
    free(json);
    return ret;
}

enum MHD_Result rotaDijkstra(struct MHD_Connection *connection)
{
    const char *origem = getParam(connection, "origem");
    const char *destino = getParam(connection, "destino");
    if (!origem || !destino)
        return responderJSON(connection, "{\"erro\":\"origem e destino obrigatorios\"}", MHD_HTTP_BAD_REQUEST);

    char org[4], dst[4];
    strncpy(org, origem, sizeof(org));
    strncpy(dst, destino, sizeof(dst));
    toUpperStrServer(org);
    toUpperStrServer(dst);

    float distancia = 0;
    int *ant = dijkstra(grafoGlobal, org, dst, &distancia);
    if (!ant)
        return responderJSON(connection, "{\"erro\":\"aeroporto nao encontrado\"}", MHD_HTTP_NOT_FOUND);

    int destIdx = buscarHash(dst);
    if (ant[destIdx] == -1 && buscarHash(org) != destIdx)
    {
        free(ant);
        return responderJSON(connection, "{\"erro\":\"caminho nao encontrado\"}", MHD_HTTP_NOT_FOUND);
    }

    int tam = 0;
    int *cam = reconstruirCaminho(grafoGlobal, ant, dst, &tam);

    char *camStr = (char *)malloc(32768);
    montarCaminhoJSON(grafoGlobal, cam, tam, camStr, 32768);

    char *json = (char *)malloc(40000);
    snprintf(json, 40000,
             "{\"caminho\":%s,\"escalas\":%d,\"distancia\":%.0f}",
             camStr, tam - 1, distancia);

    enum MHD_Result ret = responderJSON(connection, json, MHD_HTTP_OK);
    free(ant);
    free(cam);
    free(camStr);
    free(json);
    return ret;
}

enum MHD_Result rotaComparativo(struct MHD_Connection *connection)
{
    const char *origem = getParam(connection, "origem");
    const char *destino = getParam(connection, "destino");
    if (!origem || !destino)
        return responderJSON(connection, "{\"erro\":\"origem e destino obrigatorios\"}", MHD_HTTP_BAD_REQUEST);

    char org[4], dst[4];
    strncpy(org, origem, sizeof(org));
    strncpy(dst, destino, sizeof(dst));
    toUpperStrServer(org);
    toUpperStrServer(dst);

    int *antBFS = bfsMenorCaminho(grafoGlobal, org, dst);
    if (!antBFS)
        return responderJSON(connection, "{\"erro\":\"aeroporto nao encontrado\"}", MHD_HTTP_NOT_FOUND);

    int tamBFS = 0;
    int *camBFS = reconstruirCaminho(grafoGlobal, antBFS, dst, &tamBFS);
    float distBFS = calcularDistanciaCaminho(grafoGlobal, camBFS, tamBFS);

    float distDijk = 0;
    int *antDijk = dijkstra(grafoGlobal, org, dst, &distDijk);
    if (!antDijk)
    {
        free(antBFS);
        free(camBFS);
        return responderJSON(connection, "{\"erro\":\"aeroporto nao encontrado\"}", MHD_HTTP_NOT_FOUND);
    }

    int tamDijk = 0;
    int *camDijk = reconstruirCaminho(grafoGlobal, antDijk, dst, &tamDijk);

    char *camBFSStr = (char *)malloc(32768);
    char *camDijkStr = (char *)malloc(32768);
    montarCaminhoJSON(grafoGlobal, camBFS, tamBFS, camBFSStr, 32768);
    montarCaminhoJSON(grafoGlobal, camDijk, tamDijk, camDijkStr, 32768);

    char *json = (char *)malloc(80000);
    snprintf(json, 80000,
             "{\"bfs\":{\"caminho\":%s,\"escalas\":%d,\"distancia\":%.0f},"
             "\"dijkstra\":{\"caminho\":%s,\"escalas\":%d,\"distancia\":%.0f},"
             "\"economiaKm\":%.0f,\"economiaEscalas\":%d}",
             camBFSStr, tamBFS - 1, distBFS,
             camDijkStr, tamDijk - 1, distDijk,
             distBFS - distDijk, tamDijk - tamBFS);

    enum MHD_Result ret = responderJSON(connection, json, MHD_HTTP_OK);
    free(antBFS);
    free(camBFS);
    free(camBFSStr);
    free(antDijk);
    free(camDijk);
    free(camDijkStr);
    free(json);
    return ret;
}

enum MHD_Result rotaSCC(struct MHD_Connection *connection)
{
    int *scc = kosaraju(grafoGlobal);

    int maxComp = 0;
    for (int i = 0; i < grafoGlobal->numAeroportos; i++)
        if (scc[i] > maxComp)
            maxComp = scc[i];

    int *tamanhos = (int *)calloc(maxComp + 1, sizeof(int));
    for (int i = 0; i < grafoGlobal->numAeroportos; i++)
        tamanhos[scc[i]]++;

    int maior = 0, maioresQue1 = 0, isolados = 0;
    for (int i = 0; i <= maxComp; i++)
    {
        if (tamanhos[i] > maior)
            maior = tamanhos[i];
        if (tamanhos[i] > 1)
            maioresQue1++;
        if (tamanhos[i] == 1)
            isolados++;
    }

    char json[256];
    snprintf(json, sizeof(json),
             "{\"total\":%d,\"maiorComponente\":%d,\"comMaisDe1\":%d,\"isolados\":%d}",
             maxComp + 1, maior, maioresQue1, isolados);

    free(scc);
    free(tamanhos);
    return responderJSON(connection, json, MHD_HTTP_OK);
}

enum MHD_Result handler(void *cls, struct MHD_Connection *connection,
                        const char *url, const char *method,
                        const char *version, const char *upload_data,
                        size_t *upload_data_size, void **con_cls)
{
    if (strcmp(url, "/info") == 0)
        return rotaInfo(connection);
    if (strcmp(url, "/bfs") == 0)
        return rotaBFS(connection);
    if (strcmp(url, "/dijkstra") == 0)
        return rotaDijkstra(connection);
    if (strcmp(url, "/comparativo") == 0)
        return rotaComparativo(connection);
    if (strcmp(url, "/scc") == 0)
        return rotaSCC(connection);

    return responderJSON(connection, "{\"erro\":\"rota nao encontrada\"}", MHD_HTTP_NOT_FOUND);
}

struct MHD_Daemon *iniciarServidor(Grafo *grafo, int porta)
{
    grafoGlobal = grafo;
    struct MHD_Daemon *daemon = MHD_start_daemon(
        MHD_USE_THREAD_PER_CONNECTION,
        porta, NULL, NULL,
        &handler, NULL,
        MHD_OPTION_END);
    return daemon;
}