#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "routing.h"

// ******************************************************************************

const char *html_head =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n"
    "<script src='https://cdn.tailwindcss.com'></script>\n"
    "</head>\n"
    "<body>\n";

const char *html_foot =
    "\n</body>\n"
    "</html>\n";

typedef struct {
    const char *path;
    const char *content_type;
    void (*handler)(char *http_body, size_t size);
} Route;

// ******************************************************************************

void handle_response(int client_fd, char *path, char* message, int status, char *status_message, int vistor_count)
{
    static const Route routes[] = {
        {"/", "text/plain", handle_get},
        {"/about", "text/plain", handle_get_about},
        {"/cat", "text/plain", handle_get_cat},
        {"/page", "text/html", handle_get_page}
    };

    const int NUM_PATHS = sizeof(routes) / sizeof(routes[0]);
    char http_body[1024];
    const char *content_type = "text/plain";
    void (*handler)(char *http_body, size_t size);
    int route_matched = 0;

    for (int i = 0; i < NUM_PATHS; i++)
    {
        if (strcmp(path, routes[i].path) == 0)
        {
            route_matched = 1;
            content_type = routes[i].content_type;            
            handler = routes[i].handler;
            break;
        }
    }
    
    int local_status;
    char *local_status_message;

    if (route_matched == 1)
    {
        handler(http_body, sizeof(http_body));
        local_status = 200;
        local_status_message = "OK";
    }
    else
    {
        local_status = status;
        local_status_message = status_message;
        snprintf(http_body, sizeof(http_body), "%s", message);
    }

    char full_body[1152];
    snprintf(full_body, sizeof(full_body), "You are visitor #%d\n%s", vistor_count, http_body);

    char http_response[1280];
    snprintf(http_response, sizeof(http_response), "HTTP/1.1 %d %s\r\nContent-type: %s\r\nContent-length: %zu\r\n\r\n%s", local_status, local_status_message, content_type, strlen(full_body), full_body);

    write(client_fd, http_response, strlen(http_response));
}

// ******************************************************************************

void handle_get(char *http_body, size_t size)
{
    const char *message = "Home:\nthis is just the base route, I thought something should response\n";
    snprintf(http_body, size, "(>.<) Welcome to my customer HTTP server\n%s", message);
}

// ******************************************************************************

void handle_get_about(char *http_body, size_t size)
{
    const char *message = "About this server:\nGET /\nGET /about\nGET /cat\n";
    snprintf(http_body, size, "(>.<) Welcome to my customer HTTP server\n%s", message);
}

// ******************************************************************************

void handle_get_cat(char *http_body, size_t size)
{
    const char *message = "^  ^\n(>.<)\n";
    snprintf(http_body, size, "(>.<) Welcome to my customer HTTP server\n%s", message);
}

// ******************************************************************************

void handle_get_page(char *http_body, size_t size)
{
    snprintf(http_body, size,
        "%s"
        "<div><a href='https://www.youtube.com/watch?v=F2niNhQAlmE'>Guess who</a></div>"
        "%s",
        html_head, html_foot);
    printf("HTML content length: %zu\n", strlen(http_body));
}
