#ifndef ROUTING_H
#define ROUTING_H

#include <stddef.h>

void handle_response(int client_fd, char *path, char *message, int status, char *status_message, int vistor_count);
void handle_get(char *http_body, size_t size);
void handle_get_about(char *http_body, size_t size);
void handle_get_cat(char *http_body, size_t size);
void handle_get_page(char *http_body, size_t size);

#endif
