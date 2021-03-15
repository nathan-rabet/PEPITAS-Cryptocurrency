#include "server.h"
#include "client.h"
#include "network.h"
#include "../misc/safe.h"

void *accept_connection(void *arg)
{
    int clientfd = *((int *)arg);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if (getpeername(clientfd, (struct sockaddr * restrict) & client_addr, &client_addr_len) == -1)
        err(EXIT_FAILURE, "Failed to recover client IP address\n");

    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN) == NULL)
        err(EXIT_FAILURE, "Failed to convert client IP address to string\n");

    printf("New connection: '%s'\n", ip_str);

    send_client_list(clientfd);

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
    }

    return sockfd;
}

void send_client_list(int sockfd)
{
    Client *client_list = get_client();

    printf("Sending client list...\n");
    safe_write(sockfd, HD_REC_CLIENT_LIST, 20);

    for (size_t index = 0; index < MAX_NEIGHBOURS; index++)
    {
        if (client_list->neighbours[index].hostname != NULL)
        {
            safe_write(sockfd, (void *)&client_list->neighbours[index].family, sizeof(int));
            safe_write(sockfd, (void *)client_list->neighbours[index].hostname, sizeof(char) * 16);
        }
    }

    // END SENDING
    safe_write(sockfd, "\r\n\r\n", 5);
}