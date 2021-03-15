#include "client.h"
#include "server.h"
#include "network.h"

Client *get_client()
{
    static Client *client = NULL;
    if (client == NULL)
    {
        client = malloc(sizeof(Client));
        client->neighbours = malloc(MAX_NEIGHBOURS * sizeof(Neighbour));
        for (size_t i = 0; i < MAX_NEIGHBOURS; i++)
        {
            client->neighbours[i].hostname = NULL;
        }

        for (size_t i = 1; i < 12; i++)
        {
            client->neighbours[i].hostname = "192.12.1.3";
            client->neighbours[i].family = AF_INET;
        }
        
    }
    return client;
}

int set_neighbours(char *hostname, int family)
{
    Client *client = get_client();

    if (hostname == NULL)
    {
        Neighbour local;
        local.hostname = HARD_CODED_ADDR[0].hostname;
        local.family = AF_UNSPEC;
        client->neighbours[0] = local;
        return 0;
    }

    size_t index = 0;
    while (index < MAX_NEIGHBOURS)
    {
        if (client->neighbours[index].hostname == NULL)
        {
            client->neighbours[index].hostname = hostname;
            client->neighbours[index].family = family;
            index = MAX_NEIGHBOURS;
        }
        index++;
    }
    if (index == MAX_NEIGHBOURS)
    {
        return -1;
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

void wait_header(int sockfd)
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

    read_header(buffer, sockfd);
}

void read_header(char *buf, int sockfd)
{
    if (strncmp(HD_GET_BLOCKCHAIN, buf, 8) == 0)
    {
        printf("Recived header HD_GET_BLOCKCHAIN\n");
        return;
    }
    if (strncmp(HD_REC_BLOCKCHAIN, buf, 8) == 0)
    {
        printf("Recived header HD_REC_BLOCKCHAIN\n");
        return;
    }
    if (strncmp(HD_GET_CLIENT_LIST, buf, 8) == 0)
    {
        printf("Recived header HD_GET_CLIENT_LIST\n");
        send_client_list(sockfd);
        return;    
    }
    if (strncmp(HD_REC_CLIENT_LIST, buf, 8) == 0)
    {
        printf("Recived header HD_REC_CLIENT_LIST\n");
        recive_client_list(sockfd);
        return;    
    }
    
}

void recive_client_list(int sockfd)
{
    Client *client_list = get_client();
    
    void* buffer;
    size_t buffer_size;
    ssize_t nb_read;

    // Get List
    nb_read = safe_read(sockfd, (void *)&buffer, &buffer_size);

    ssize_t buffer_index = 0;

    for(size_t index = 0; index < MAX_NEIGHBOURS; index++)
    {
        if (client_list->neighbours[index].hostname == NULL)
        {
            client_list->neighbours[index].family = *(int *)(buffer + buffer_index);
            printf("Family: %i", *(int *)(buffer + buffer_index));
            buffer_index += sizeof(int);
            client_list->neighbours[index].hostname = (char *)(buffer + buffer_index);
            printf("  IP: %s\n", (char *)(buffer + buffer_index));
            buffer_index += sizeof(char) * 16;
            if (buffer_index >= nb_read-6)
                break;
        }
    }

    free(buffer);
    
}