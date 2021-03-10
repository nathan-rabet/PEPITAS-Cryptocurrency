#include "network.h"

int connect_to_network()
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    
    hints.ai_family = AF_INET;       //IPV4 only
    hints.ai_socktype = SOCK_STREAM; //TCP

    // Foreach potential client `i` in the client_list
    for (size_t i = 0; i < len; i++)
    {
        struct addrinfo *result;
        int addrinfo_ret;

        ClientData client = client_list[i];

        // Get adress information
        addrinfo_ret = getaddrinfo(client.hostname, CLIENT_PORT, &hints, &result);

        // If adress information fetching failed
        if (addrinfo_ret != 0)
        {
            errx(EXIT_FAILURE, "Fail getting address for %s on port %s: %s",
                 client.hostname, CLIENT_PORT, gai_strerror(addrinfo_ret));
        }

        // try to connect for each result

        int sockfd;
        struct addrinfo *rp; // result points to a linked list
        for (rp = result; rp != NULL; rp = rp->ai_next)
        {
            sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            if (sockfd == -1)
                continue; // The socket is not created
            // Try to connect
            if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
                break;
            // Fail to connect
            close(sockfd);
        }

        freeaddrinfo(result);

        if (rp != NULL)
        {
            printf("Connection successfull!\n");
            return sockfd;
        }
    }
}

void get_client_data(int sockfd)
{
    printf("Waiting for list...\n");
    ssize_t nb_read;
    ClientData client;

    char buff[BUF_SIZE];

    while ((nb_read = read(sockfd, &buff, BUF_SIZE)) != 0)
    {
        if (nb_read == -1)
        {
            printf("Error to read message.\n");
            break;
        }

        write(STDOUT_FILENO, &buff, nb_read);
    }
    printf("Connection closed\n");
    return client;
}