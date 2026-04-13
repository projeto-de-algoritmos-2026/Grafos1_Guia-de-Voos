#ifndef SERVER_H
#define SERVER_H

#include "graph.h"
#include <microhttpd.h>

struct MHD_Daemon *iniciarServidor(Grafo *grafo, int porta);

enum MHD_Result handler(void *cls, struct MHD_Connection *connection,
                        const char *url, const char *method,
                        const char *version, const char *upload_data,
                        size_t *upload_data_size, void **con_cls);

#endif