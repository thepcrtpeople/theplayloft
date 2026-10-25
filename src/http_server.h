#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <stdint.h>

#define HTTP_PORT 8080
#define HTTP_MAX_CLIENTS 8
#define HTTP_BUFFER_SIZE 4096

typedef struct {
    int listen_fd;
    int running;
} http_server_t;

int http_server_start(http_server_t *server);
void http_server_stop(http_server_t *server);
void http_server_run_loop(http_server_t *server);

#endif
