#include "network.h"


int ConnectionToNetwork(char* name, char* port)
{
    // Try to connect to the peer-to-peer network

    struct addrinfo hints;      //
    struct addrinfo *result;    //
    struct addrinfo *rp;    //
    int addrinfo_error;
    int sockfd;                 //File Descriptor of the socket

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;      //IPV4 only
    hints.ai_socktype = SOCK_STREAM;      //TCP

    // Get info
    addrinfo_error = getaddrinfo(name, port, &hints, &result);
    
    // Error management
    if (addrinfo_error != 0)
    {
        errx(EXIT_FAILURE, "Fail getting address fo %s on port %s: %s", 
        name, port, gai_strerror(addrinfo_error));
    }

    // result points to a linked list
    // try yo connect for each result
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1) continue;         // The socket is not created
        // Try to connect
        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1) break;
        // Fail to connect
        close(sockfd);
    }
    
    freeaddrinfo(result);

    if (rp == NULL)
    {
        printf("Connection failed to %s on port %s\n", name, port);
        return -1;
    }
    printf("Connection successfull!\n");
    return sockfd;
}