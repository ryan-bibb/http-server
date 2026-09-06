/*
TODOs: 
    - use fopen/fread to actually open that are on the machine instead of hardcoded strings
    - add multi-threaded logic
    - add more request parsing
    - read timeout for client that doesnt send data
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "server.h"
#include "routing.h"

void handle_request(int client_fd, char *buffer, ssize_t bytes_read);

int vistor_count = 0;

int main()
{
    signal(SIGPIPE, SIG_IGN); // a client disconnecting mid-write must not kill the whole server

    int fd = create_server_socket(8080);

    if (fd == -1) 
    {
        return 1;
    }

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept_client(fd, &client_addr, &client_addr_len);

        if (client_fd == -1)
        {
            continue;
        }

        char buffer[4096];
        ssize_t bytes_read = read(client_fd, buffer, sizeof(buffer) - 1); // leave room for null term
        
        handle_request(client_fd, buffer, bytes_read);
        close(client_fd);
    }
}


void handle_request(int client_fd, char* buffer, ssize_t bytes_read)
{
    if (bytes_read == -1)
    {
        perror("read error");
    }
    else
    {
        char http_method[8];
        char http_path[256];
        buffer[bytes_read] = '\0';
 
        int get_request = sscanf(buffer, "%7s %255s", http_method, http_path); 
 
        
        if (get_request == 2) 
        {
            printf("(>.<) We got a request ... (>.<)\n");
            printf("HTTP method: \n%s\n", http_method);
            printf("Path:\n%s\n", http_path);
            printf("\n=================================\n");
            vistor_count++;
            
            const char *valid_methods[] = {"GET", "POST", "PUT", "PATCH", "DELETE", "HEAD", "OPTIONS"};
            int is_valid_method = 0; // 1 = true -- 0 = false

            for (int i = 0; i < sizeof(valid_methods) / 8; i++)
            {
                if (strcmp(http_method, valid_methods[i]) == 0)
                {
                    is_valid_method = 1;
                    break;
                }
            }

            if (is_valid_method == 1)
            {
                if (strcmp(http_method, "GET") == 0)
                {
                    handle_response(client_fd, http_path, "Invalid path\n", 404, "Not Found", vistor_count);
                }
                else if (strcmp(http_method, "POST") == 0)
                {
                    // handle_response(client_fd, "I havent built this yet\n", 200, "OK");
                }
                else if (strcmp(http_method, "PUT") == 0)
                {
                    // handle_response(client_fd, "I havent built this yet\n", 200, "OK");
                }
                else if (strcmp(http_method, "PATCH") == 0)
                {
                    // handle_response(client_fd, "I havent built this yet\n", 200, "OK");
                }
                else if (strcmp(http_method, "DELETE") == 0)
                {
                    // handle_response(client_fd, "I havent built this yet\n", 200, "OK");
                }
                else if (strcmp(http_method, "HEAD") == 0)
                {
                    // handle_response(client_fd, "I havent built this yet\n", 200, "OK");
                }
                else // OPTIONS
                {
                    // handle_response(client_fd, "I havent built this yet\n", 200, "OK");
                }
            }
            else
            {
                printf("Invalid request method: %s\n", http_method);
                handle_response(client_fd, "", "Invalid HTTP method\n", 400, "Bad request", vistor_count);
            }
        }
        else
        {
            printf("Could not parse request :( \n");
        }    
   }
}

