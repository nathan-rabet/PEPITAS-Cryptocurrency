#include "network/get_data.h"

int process_header(char *header, size_t size, int sockfd, infos_st *infos)
{
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
    if (strncmp(HD_SEND_CLIENT_LIST, header, 8) == 0)
    {
        printf("Recived header HD_SEND_CLIENT_LIST\n");
        return fetch_client_list(IM_CLIENT, sockfd);
    }
    if (strncmp(HD_GET_BLOCKS, header, strlen(HD_GET_BLOCKS)) == 0)
    {
        printf("Recived header HD_GET_BLOCKS\n");
        return read_get_blocks(sockfd, infos);
    }
    if (strncmp(HD_ACTUAL_HEIGHT, header, strlen(HD_ACTUAL_HEIGHT)) == 0)
    {
        printf("Recived header HD_ACTUAL_HEIGHT\n");
        return read_actual_height(header);
    }
    if (strncmp(HD_SEND_BLOCK, header, strlen(HD_SEND_BLOCK)) == 0)
    {
        printf("Recived header HD_SEND_BLOCK\n");
        return read_send_block(sockfd);
    }
    if (strncmp(HD_GET_PENDING_TRANSACTION_LIST, header, strlen(HD_GET_PENDING_TRANSACTION_LIST)) == 0)
    {
        printf("Recived header HD_GET_PENDING_TRANSACTION_LIST\n");
        send_pending_transaction_list(sockfd);
        return 0;
    }
    if (strncmp(HD_REJECT_DEMAND, header, strlen(HD_REJECT_DEMAND)) == 0)
    {
        printf("Recived header HD_REJECT_DEMAND\n");
        send_reject_demand(sockfd);
        return 0;
    }
    if (strncmp(HD_SEND_PENDING_TRANSACTION, header, strlen(HD_SEND_PENDING_TRANSACTION)) == 0)
    {
        printf("Recived header HD_SEND_PENDING_TRANSACTION\n");
        return read_pending_transaction_list(sockfd);
    }
    if (strncmp(HD_SEND_EPOCH_BLOCK, header, strlen(HD_SEND_EPOCH_BLOCK)) == 0)
    {
        printf("Recived header HD_SEND_EPOCH_BLOCK\n");
        return read_epoch_block(sockfd);
    }
    if (strncmp(HD_SEND_VOTE, header, strlen(HD_SEND_VOTE)) == 0)
    {
        printf("Recived header HD_SEND_EPOCH_BLOCK\n");
        return read_vote(sockfd);
    }
    


    return 0;
}

int fetch_client_list(char who, int fd)
{
    size_t index = 0;
    int family = 0;
    char *hostname = malloc(SIZE_OF_HOSTNAME);

    while (index < MAX_NEIGHBOURS)
    {
        read(fd, &family, sizeof(int));

        int hostname_size;
        // IPv4
        if (family == AF_INET)
            hostname_size = 15;
        // IPv6
        else
            hostname_size = 39;

        read(fd, &hostname, hostname_size);
        index++;
        
        set_neighbour(who, hostname, family);
    }
    free(hostname);
    return 0;
}


int read_header(int sockfd, infos_st *infos)
{
    // Waiting header for server and read it
    char *buffer;
    size_t buffer_size;
    ssize_t nb_read;

    // Get Header
    nb_read = safe_read(sockfd, (void *)&buffer, &buffer_size);

    if (nb_read != -1)
    {
        int r = process_header(buffer, nb_read, sockfd, infos);
        free(buffer);
        return r;
    }
    return -1;
}

int read_get_blocks(int fd, infos_st *infos){
    uint32_t version;
    read(fd, &version, sizeof(uint32_t));
    char hash_count;
    read(fd, &hash_count, sizeof(char));
    for (char i = 0; i < hash_count; i++)
    {
        size_t height;
        read(fd, &height, sizeof(uint32_t));
        if (height == 0){
            send_actual_height(fd, infos);
        }
        else
        {
            if (height < infos->actual_height){
                send_send_block(fd, height);
            }
            else
            {
                send_reject_demand(fd);
            }
        }
    }
    return 0;
}

size_t read_actual_height(int fd){
    size_t ac;
    read(fd, &ac, sizeof(size_t));
    return ac;
}

int read_send_block(int fd){
    Block my_new_block;
    convert_data_to_block(&my_new_block, fd);
    // ADD TO BLOCKCHAIN IF TRUE
    return 0;
}

int read_vote(int fd){
    
    return 0;
}

int read_epoch_block(int fd){
    Block my_new_epoch;
    convert_data_to_block(&my_new_epoch, fd);
    // GESTION EPOCH
    return 0;
}

int read_pending_transaction_list(int fd){
    return 0;
}