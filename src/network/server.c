#include "network.h"


int init_server(char* name)
{
    // Try to connect to the peer-to-peer network

    struct addrinfo hints;      //
    struct addrinfo *result;    //
    struct addrinfo *rp;    //
    int addrinfo_error;
    int sockfd;                 //File Descriptor of the socket
    int clientfd;                 //File Descriptor of the client

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;      //IPV4 only
    hints.ai_socktype = SOCK_STREAM;      //TCP
    hints.ai_flags = AI_PASSIVE;      //Server
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    // Get info
    addrinfo_error = getaddrinfo(name, SERVER_PORT, &hints, &result);
    
    // Error management
    if (addrinfo_error != 0)
    {
        errx(EXIT_FAILURE, "Fail getting address fo %s on port %s: %s", 
        name, SERVER_PORT, gai_strerror(addrinfo_error));
    }

    // result points to a linked list
    // try yo connect for each result
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1) continue;         // The socket is not created
        // Try to connect
        if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
        {
            // Success
            break;
        }
        close(sockfd);
    }
    
    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);

    while (1)
    {
        clientfd = accept(sockfd, rp->ai_addr, &rp->ai_addrlen);
        if (clientfd == -1)
        {
            printf("Error to accept\n");
            break;
        }

        
        ClientData cl;
        cl.family = AF_INET6;
        cl.hostname = "salam";
        write(clientfd, &cl, sizeof(cl));
        
        close(clientfd);
    }
    
    return sockfd;
}