#include "client.h"
#include "network.h"

Client *get_client()
{
    static Client *client = NULL;
    if (client == NULL)
    {
        client = malloc(sizeof(Client));
        client->neighbours = malloc(MAX_NEIGHBOURS * sizeof(Neighbour));
    }
    return client;
}

int set_neighbours()
{
    Client *client = get_client();
    // TODO : Set neighbours properly !

    if (client->neighbours[0].hostname == NULL)
    {

        Neighbour local;

        local.hostname = STATIC_DNS;
        local.family = AF_UNSPEC;
        client->neighbours[0] = local;
    }
    return 0;
}

int connect_to_network(int client_to_connect_id)
{
    Client *client = get_client();
    struct addrinfo hints = {0};
    Neighbour neighbour = client->neighbours[client_to_connect_id];
    hints.ai_family = neighbour.family; //IPV4 only
    hints.ai_socktype = SOCK_STREAM;    //TCP

    struct addrinfo *result;
    int addrinfo_ret;

    // If no neighbour
    if (neighbour.hostname == NULL)
        return -1;

    // Get adress information
    addrinfo_ret = getaddrinfo(neighbour.hostname, STATIC_PORT, &hints, &result);

    // If adress information fetching failed
    if (addrinfo_ret != 0)
    {
        errx(EXIT_FAILURE, "Fail getting information for address '%s' on port %s: %s",
             neighbour.hostname, STATIC_PORT, gai_strerror(addrinfo_ret));
    }

    // Try to connect for each result
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
        // Connection success
        return sockfd;
    }

    // Connection failed
    printf("Failed to connect at %s\n", neighbour.hostname);
    return -1;
}

void wait_server_header(int sockfd)
{
    // Waiting header for server and read it

    char *buffer;
    size_t buffer_size;
    ssize_t nb_read;

    // Get Header
    nb_read = safe_read(sockfd, (void *)&buffer, &buffer_size);

    if (nb_read == -1)
    {
        printf("Failed to read message.\n");
        return;
    }

    read_header(buffer);
}

void read_header(char *buf)
{
    if (strncmp("IM_AWAKE", buf, 8) == 0)
    {
        printf("Recived header IM_AWAKE\n");    
    }
    
}