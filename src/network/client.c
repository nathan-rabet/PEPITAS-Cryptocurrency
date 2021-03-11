#include "network.h"

int set_neighbours()
{
    // TODO : Set neighbours properly !

    if (client.neighbours[0].hostname == 0) {

        Neighbour local;

        local.hostname = "localhost";
        client.neighbours[0] = local;

    }
    return 0;
}

int connect_to_network()
{
    struct addrinfo hints = {0};

    hints.ai_family = AF_INET;       //IPV4 only
    hints.ai_socktype = SOCK_STREAM; //TCP

    // Foreach potential client `i` in the client_list
    for (size_t i = 0; i < MAX_NEIGHBOURS_NB; i++)
    {
        struct addrinfo *result;
        int addrinfo_ret;

        Neighbour neighbour = client.neighbours[i];

        // If no neighbour
        if (neighbour.hostname == NULL)
            continue;

        // Get adress information
        addrinfo_ret = getaddrinfo(neighbour.hostname, CLIENT_PORT, &hints, &result);

        // If adress information fetching failed
        if (addrinfo_ret != 0)
        {
            errx(EXIT_FAILURE, "Fail getting information for address '%s' on port %s: %s",
                 neighbour.hostname, CLIENT_PORT, gai_strerror(addrinfo_ret));
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
    // TODO : Clear Maxence's shit
    printf("Waiting for list...\n");
    ssize_t nb_read;
    Neighbour client;

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
}