#include "network/client.h"
#include "network/server.h"
#include "network/network.h"
#include "network/send_data.h"
#include "network/get_data.h"

int process_header(char *header, size_t size, int sockfd, char type)
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
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        if (getpeername(sockfd, (struct sockaddr * restrict) & client_addr, &client_addr_len) == -1)
            err(EXIT_FAILURE, "Failed to recover client IP address\n");

        char ip_str[39];
        if (inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN) == NULL)
            err(EXIT_FAILURE, "Failed to convert client IP address to string\n");
        return send_client_list(IM_SERVER, sockfd, ip_str);
    }
    if (strncmp(HD_SEND_CLIENT_LIST, header, 8) == 0)
    {
        printf("Recived header HD_SEND_CLIENT_LIST\n");
        return fetch_client_list(IM_CLIENT, header, size, type);
    }

    if (strncmp(HD_CONNECTION_TO_NETWORK, header, strlen(HD_CONNECTION_TO_NETWORK)) == 0)
    {
        printf("Recived header HD_CONNECTION_TO_NETWORK\n");
        return 1;
    }
    if (strncmp(HD_CONNECTION_TO_NODE, header, strlen(HD_CONNECTION_TO_NODE)) == 0)
    {
        printf("Recived header HD_CONNECTION_TO_NODE\n");
        return 1;
    }
    return 0;
}

int fetch_client_list(char who, char *buffer, size_t buffer_size, char type)
{
    size_t buffer_index = strlen(HD_SEND_CLIENT_LIST);

    int family = 0;
    char *hostname = malloc(SIZE_OF_HOSTNAME);

    while (buffer_index < buffer_size - 5)
    {
        family = *(int *)(buffer + buffer_index);

        buffer_index += sizeof(int);

        int hostname_size;
        // IPv4
        if (family == AF_INET)
            hostname_size = 15;
        // IPv6
        else
            hostname_size = 39;

        memcpy(hostname, buffer + buffer_index, hostname_size);

        buffer_index += hostname_size;
        
        set_neighbour(who, hostname, family);
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
        return process_header(buffer, nb_read, sockfd, NODESERVER);
    return -1;
}