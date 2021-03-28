#include "network/client.h"
#include "network/server.h"
#include "network/network.h"
#include "network/get_data.h"

int process_header(char *header, int sockfd)
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
        return fetch_client_list(sockfd);
    }
    return 0;
}

int fetch_client_list(int neighbour_id)
{
    Node *my_node = get_my_node();

    void *buffer;
    size_t buffer_size;
    ssize_t nb_read;

    // Ask for client list
    if (safe_write(my_node->neighbours[neighbour_id].client_sockfd,
                   HD_GET_CLIENT_LIST, sizeof(HD_GET_CLIENT_LIST)) == -1)
    {
        dprintf(STDERR_FILENO, "Failed to ask for the client list to node '%d', deleting it...", neighbour_id);
        memset(&my_node->neighbours[neighbour_id], 0, sizeof(Neighbour));
        return -1;
    }

    // Get the list
    nb_read = safe_read(my_node->neighbours[neighbour_id].client_sockfd, (void *)&buffer, &buffer_size);

    if (nb_read == -1)
    {
        dprintf(STDERR_FILENO, "Error while reading socket data");
        return -1;
    }

    ssize_t buffer_index = 0;

    for (size_t index = 0; index < MAX_NEIGHBOURS; index++)
    {
        if (my_node->neighbours[index].hostname == NULL)
        {
            my_node->neighbours[index].family = *(int *)(buffer + buffer_index);
            printf("Family: %i", *(int *)(buffer + buffer_index));

            buffer_index += sizeof(int);

            my_node->neighbours[index].hostname = (char *)(buffer + buffer_index);
            printf("\tIP: %s\n", (char *)(buffer + buffer_index));

            int hostname_size_plus_one;
            // IPv4 + 1
            if (my_node->neighbours[index].family == INET_ADDRSTRLEN)
                hostname_size_plus_one = 16;
            // IPv6 + 1
            else
                hostname_size_plus_one = 40;
            buffer_index += sizeof(char) * hostname_size_plus_one;
            if (buffer_index >= nb_read - 6)
                break;
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
        return process_header(buffer, sockfd);
    return -1;
}