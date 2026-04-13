#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 17576

int tabelaHash[HASH_SIZE];

int hashIata(const char *iata)
{
    if (iata[0] < 'A' || iata[1] < 'A' || iata[2] < 'A')
        return -1;
    return (iata[0] - 'A') * 676 +
           (iata[1] - 'A') * 26 +
           (iata[2] - 'A');
}

void inicializarHash()
{
    for (int i = 0; i < HASH_SIZE; i++)
        tabelaHash[i] = -1;
}

void inserirHash(const char *iata, int indice)
{
    int h = hashIata(iata);
    if (h >= 0)
        tabelaHash[h] = indice;
}

int buscarHash(const char *iata)
{
    int h = hashIata(iata);
    if (h < 0)
        return -1;
    return tabelaHash[h];
}

void removerAspas(char *str)
{
    if (str[0] == '"')
        memmove(str, str + 1, strlen(str));
    int len = strlen(str);
    if (str[len - 1] == '"')
        str[len - 1] = '\0';
}

void carregarAeroportos(Grafo *grafo, const char *caminho)
{
    inicializarHash();
    FILE *file = fopen(caminho, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo de aeroportos");
        return;
    }
    char linha[512];
    while (fgets(linha, sizeof(linha), file))
    {
        char *token = strtok(linha, ",");
        int campo = 0;
        int pular = 0;
        Aeroporto aeroporto;

        while (token != NULL)
        {
            switch (campo)
            {
            case 1:
                removerAspas(token);
                strncpy(aeroporto.nome, token, sizeof(aeroporto.nome));
                break;
            case 4:
                if (strcmp(token, "\\N") == 0)
                    pular = 1;
                else
                {
                    removerAspas(token);
                    strncpy(aeroporto.iata, token, sizeof(aeroporto.iata));
                }
                break;
            case 6:
                aeroporto.latitude = atof(token);
                break;
            case 7:
                aeroporto.longitude = atof(token);
                break;
            }
            token = strtok(NULL, ",");
            campo++;
        }

        if (!pular)
        {
            inserirHash(aeroporto.iata, grafo->numAeroportos);
            criarAeroporto(grafo, aeroporto);
        }
    }

    fclose(file);
}

void carregarRotas(Grafo *grafo, const char *caminho)
{
    FILE *file = fopen(caminho, "r");
    if (file == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    char linha[512];
    while (fgets(linha, sizeof(linha), file))
    {
        char *token = strtok(linha, ",");
        int campo = 0;
        int pular = 0;

        char iataOrigem[4], iataDestino[4];

        while (token != NULL)
        {
            switch (campo)
            {
            case 2:
                strncpy(iataOrigem, token, sizeof(iataOrigem));
                break;
            case 4:
                strncpy(iataDestino, token, sizeof(iataDestino));
                break;
            default:
                break;
            }
            token = strtok(NULL, ",");
            campo++;
        }
        int origem = buscarHash(iataOrigem);
        int destino = buscarHash(iataDestino);

        if (origem != -1 && destino != -1)
        {
            float peso = haversine(
                grafo->aeroportos[origem].latitude,
                grafo->aeroportos[origem].longitude,
                grafo->aeroportos[destino].latitude,
                grafo->aeroportos[destino].longitude);
            criarVoo(grafo, origem, destino, peso);
        }
    }
    fclose(file);
}