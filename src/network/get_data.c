#include "network/client.h"
#include "network/server.h"
#include "network/network.h"
#include "network/send_data.h"
#include "network/get_data.h"

int process_header(char *header, size_t size, int sockfd)
{
    if (strncmp(HD_GET_BLOCKCHAIN, header, strlen(HD_GET_BLOCKCHAIN)) == 0)
    {
        printf("Recived header HD_GET_BLOCKCHAIN\n");
        return 0;
    }
    if (strncmp(HD_SEND_BLOCKCHAIN, header, strlen(HD_SEND_BLOCKCHAIN)) == 0)
    {
        printf("Recived header HD_SEND_BLOCKCHAIN\n");
        return 0;
    }
    if (strncmp(HD_GET_CLIENT_LIST, header, strlen(HD_GET_CLIENT_LIST)) == 0)
    {
        printf("Recived header HD_GET_CLIENT_LIST\n");
        return send_client_list(sockfd);
    }
    if (strncmp(HD_SEND_CLIENT_LIST, header, 8) == 0)
    {
        printf("Recived header HD_SEND_CLIENT_LIST\n");
        return fetch_client_list(header, size);
    }

    if (strncmp(HD_CONNECTION_TO_NETWORK, header, strlen(HD_CONNECTION_TO_NETWORK)) == 0)
    {
        printf("Recived header HD_CONNECTION_TO_NETWORK\n");
        return 1;
    }
    return 0;
}

int fetch_client_list(char *buffer, size_t buffer_size)
{
    Node *my_node = get_my_node();

    size_t buffer_index = strlen(HD_SEND_CLIENT_LIST);

    int family = 0;
    char *hostname = malloc(SIZE_OF_HOSTNAME);

    printf("%lu\n", buffer_size);

    while (buffer_index < buffer_size - 5)
    {
        family = *(int *)(buffer + buffer_index);
        printf("Family: %i", family);

        buffer_index += sizeof(int);

        int hostname_size;
        // IPv4
        if (family == AF_INET)
            hostname_size = 15;
        // IPv6
        else
            hostname_size = 39;

        memcpy(hostname, buffer + buffer_index, hostname_size);
        printf("\tIP: %s\n", hostname);
        
        buffer_index += hostname_size;
        
        set_neighbour(hostname, family);
    }
    


    for (size_t index = 0; index < MAX_NEIGHBOURS && buffer_index < buffer_size; index++)
    {
        if (my_node->neighbours[index].hostname == NULL)
        {
        }
    }

    free(buffer);
    return 0;
}


int read_header(int sockfd)
{
    // Waiting header for server and read it
    char *buffer;
    size_t buffer_size;
    ssize_t nb_read;

    // Get Header
    nb_read = safe_read(sockfd, (void *)&buffer, &buffer_size);

    if (nb_read != -1)
        return process_header(buffer, nb_read, sockfd);
    return -1;
}