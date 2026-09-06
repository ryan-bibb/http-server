#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>

int create_server_socket(int port);
int accept_client(int server_fd, struct sockaddr_in *client_addr, socklen_t *client_addr_len);

#endif
