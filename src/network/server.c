#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <err.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

#include "server.h"
#include "network.h"
#include "../misc/safe.h"

void *accept_connection(void *arg)
{
    int clientfd = *((int *)arg);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len;

    getsockname(clientfd, (struct sockaddr *)&client_addr, &client_addr_len);

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN);

    printf("New connection: '%s'\n", ip_str);

    server_im_awake(clientfd);

    close(clientfd);
    free(arg);

    return NULL;
}

int init_server()
{
    // Try to connect to the peer-to-peer network

    struct addrinfo hints = {0}; //
    struct addrinfo *result;     //
    struct addrinfo *rp;         //
    int addrinfo_error;
    int sockfd; //File Descriptor of the socket

    hints.ai_family = AF_UNSPEC;     //IPV4 only
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE;     //Server
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    // Get info
    addrinfo_error = getaddrinfo(NULL, STATIC_PORT, &hints, &result);

    // Error management
    if (addrinfo_error != 0)
    {
        errx(EXIT_FAILURE, "Fail getting address on port %s: %s",
             STATIC_PORT, gai_strerror(addrinfo_error));
    }

    // result points to a linked list
    // try yo connect for each result
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sockfd = socket(rp->ai_family, rp->ai_socktype, 0);
        if (sockfd == -1)
            continue; // The socket is not created

        int chezpas = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &chezpas, sizeof(int));

        // Try to connect
        if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == -1)
        {
            close(sockfd);
            break;
        }
        break;
    }

    if (rp == NULL)
    { /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);

    while (1)
    {
        int *clientfd = malloc(sizeof(int));
        *clientfd = accept(sockfd, rp->ai_addr, &rp->ai_addrlen);
        if (*clientfd != -1)
        {
            pthread_t thread;
            pthread_create(&thread, NULL, accept_connection, clientfd);
        }
        clientfd = NULL;
    }

    return sockfd;
}

int server_im_awake(int sockfd)
{
    return safe_write(sockfd, "IM_AWAKE\r\n\r\n", 13);
}