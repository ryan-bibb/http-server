#include <stdio.h>
#include <arpa/inet.h>
#include "server.h"

int create_server_socket(int port)
{
    int domain = AF_INET;
    int type = SOCK_STREAM;
    int protocol = 0;
    int fd = socket(domain, type, protocol);

    if (fd == -1)
    {
        perror("socket error");
        return -1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY; // this will allow any machine on LAN to access the server

    int bind_result = bind(fd, (struct sockaddr *)&addr, sizeof(addr));

    if (bind_result == -1)
    {
        perror("bind error");
        return -1;
    }

    int listen_result = listen(fd, 10);

    if (listen_result == -1)
    {
        perror("listen error");
        return -1;
    }

    return fd;
}

int accept_client(int server_fd, struct sockaddr_in *client_addr, socklen_t *client_addr_len)
{
    int client_fd = accept(server_fd, (struct sockaddr *)client_addr, client_addr_len);

    if (client_fd == -1)
    {
        perror("accept error");
        return -1;
    }

    return client_fd;
}
