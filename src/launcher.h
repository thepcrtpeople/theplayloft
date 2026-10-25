#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "http_server.h"
#include "launcher.h"
#include "page.h"

static void send_response(int client_fd, const char *status, const char *content_type, const char *body) {
    char header[512];
    int len = (int)strlen(body);
    snprintf(header, sizeof(header),
             "HTTP/1.1 %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Connection: close\r\n\r\n",
             status, content_type, len);
    send(client_fd, header, strlen(header), 0);
    send(client_fd, body, len, 0);
}

static void handle_request(int client_fd, const char *request) {
    if (strstr(request, "GET / ") != NULL || strstr(request, "GET /index.html") != NULL) {
        send_response(client_fd, "200 OK", "text/html", waiting_page_html);
        return;
    }

    if (strstr(request, "POST /launch") != NULL) {
        char *body = strstr(request, "\r\n\r\n");
        if (body != NULL) {
            body += 4;
            launch_result_t result = launch_game_from_request(body);
            if (result.success) {
                char response[256];
                snprintf(response, sizeof(response),
                         "{\"status\":\"ok\",\"message\":\"%s\"}",
                         result.message);
                send_response(client_fd, "200 OK", "application/json", response);
            } else {
                char response[256];
                snprintf(response, sizeof(response),
                         "{\"status\":\"error\",\"message\":\"%s\"}",
                         result.message);
                send_response(client_fd, "400 Bad Request", "application/json", response);
            }
            return;
        }
    }

    if (strstr(request, "POST /stop") != NULL) {
        stop_current_session();
        send_response(client_fd, "200 OK", "application/json", "{\"status\":\"ok\",\"message\":\"Session stopped\"}");
        return;
    }

    send_response(client_fd, "404 Not Found", "text/plain", "Not found");
}

int http_server_start(http_server_t *server) {
    if (server == NULL) {
        return -1;
    }

    server->listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->listen_fd < 0) {
        perror("socket");
        return -1;
    }

    int opt = 1;
    setsockopt(server->listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(HTTP_PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server->listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server->listen_fd);
        return -1;
    }

    if (listen(server->listen_fd, HTTP_MAX_CLIENTS) < 0) {
        perror("listen");
        close(server->listen_fd);
        return -1;
    }

    server->running = 1;
    return 0;
}

void http_server_stop(http_server_t *server) {
    if (server != NULL) {
        server->running = 0;
        if (server->listen_fd >= 0) {
            close(server->listen_fd);
            server->listen_fd = -1;
        }
    }
}

void http_server_run_loop(http_server_t *server) {
    if (server == NULL || !server->running) {
        return;
    }

    while (server->running) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server->listen_fd, (struct sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            continue;
        }

        char buffer[HTTP_BUFFER_SIZE];
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            handle_request(client_fd, buffer);
        }

        shutdown(client_fd, SHUT_RDWR);
        close(client_fd);
    }
}
