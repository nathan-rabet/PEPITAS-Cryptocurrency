#include "network/get_data.h"

size_t process_header(char *header, int sockfd, infos_st *infos)
{
    if (strncmp(HD_GET_CLIENT_LIST, header, strlen(HD_GET_CLIENT_LIST)) == 0)
    {
        SERVERMSG
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
        SERVERMSG
        printf("Recived header HD_CONNECTION_TO_NETWORK\n");
        return 1;
    }
    if (strncmp(HD_CONNECTION_TO_NODE, header, strlen(HD_CONNECTION_TO_NODE)) == 0)
    {
        SERVERMSG
        printf("Recived header HD_CONNECTION_TO_NODE\n");
        return 1;
    }
    if (strncmp(HD_SEND_CLIENT_LIST, header, 8) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_CLIENT_LIST\n");
        return fetch_client_list(IM_CLIENT, sockfd);
    }
    if (strncmp(HD_GET_BLOCKS, header, strlen(HD_GET_BLOCKS)) == 0)
    {
        SERVERMSG
        printf("Recived header HD_GET_BLOCKS\n");
        return read_get_blocks(sockfd, infos);
    }
    if (strncmp(HD_ACTUAL_HEIGHT, header, strlen(HD_ACTUAL_HEIGHT)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_ACTUAL_HEIGHT\n");
        return read_actual_height(sockfd);
    }
    if (strncmp(HD_SEND_BLOCK, header, strlen(HD_SEND_BLOCK)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_BLOCK\n");
        read_send_block(sockfd);
        return 0;
    }
    if (strncmp(HD_GET_PENDING_TRANSACTION_LIST, header, strlen(HD_GET_PENDING_TRANSACTION_LIST)) == 0)
    {
        SERVERMSG
        printf("Recived header HD_GET_PENDING_TRANSACTION_LIST\n");
        send_pending_transaction_list(sockfd);
        return 0;
    }
    if (strncmp(HD_SEND_PENDING_TRANSACTION_LIST, header, strlen(HD_SEND_PENDING_TRANSACTION_LIST)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_PENDING_TRANSACTION_LIST\n");
        read_send_pending_transaction_list(sockfd, infos);
        return 0;
    }
    if (strncmp(HD_REJECT_DEMAND, header, strlen(HD_REJECT_DEMAND)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_REJECT_DEMAND\n");
        return 0;
    }
    if (strncmp(HD_SEND_PENDING_TRANSACTION, header, strlen(HD_SEND_PENDING_TRANSACTION)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_PENDING_TRANSACTION\n");
        return read_send_pending_transaction(sockfd);
    }
    if (strncmp(HD_GET_PENDING_TRANSACTION, header, strlen(HD_GET_PENDING_TRANSACTION)) == 0)
    {
        SERVERMSG
        printf("Recived header HD_GET_PENDING_TRANSACTION\n");
        return read_get_pending_transaction(sockfd);
    }
    if (strncmp(HD_SEND_EPOCH_BLOCK, header, strlen(HD_SEND_EPOCH_BLOCK)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_EPOCH_BLOCK\n");
        return read_epoch_block(sockfd);
    }
    if (strncmp(HD_SEND_VOTE, header, strlen(HD_SEND_VOTE)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_EPOCH_BLOCK\n");
        return read_vote(sockfd);
    }
    

    // WARNINGMSG("Can't read header!")

    return 0;
}

int fetch_client_list(char who, int fd)
{
    int index = 0;
    int family = 0;
    char *hostname = malloc(SIZE_OF_HOSTNAME);
    int nb_ng;
    read(fd, &nb_ng, sizeof(int));

    while (index < nb_ng)
    {
        read(fd, &family, sizeof(int));

        int hostname_size;
        // IPv4
        if (family == AF_INET)
            hostname_size = 15;
        // IPv6
        else
            hostname_size = 39;

        read(fd, hostname, hostname_size);
        index++;
        
        set_neighbour(who, hostname, family);
    }
    free(hostname);
    return 0;
}


size_t read_header(int sockfd, infos_st *infos)
{
    // Waiting header for server and read it
    char *buffer;
    size_t buffer_size;
    ssize_t nb_read;

    // Get Header
    nb_read = safe_read(sockfd, (void *)&buffer, &buffer_size);

    if (nb_read != -1)
    {
        size_t r = process_header(buffer, sockfd, infos);
        free(buffer);
        return r;
    }
    return 0;
}

int read_get_blocks(int fd, infos_st *infos){
    get_blocks_t demand;
    ssize_t r;
    if ((r = recv(fd, &demand, sizeof(get_blocks_t), MSG_WAITALL)) != sizeof(get_blocks_t))
    {
        SERVERMSG
        printf("OOF Failed to load payload: %lx\n", r);
    }
    for (char i = 0; i < demand.nb_demands; i++)
    {
        size_t height = *(demand.blocks_height + i);
        if (height == 0){
            send_actual_height(fd, infos);
        }
        else
        {
            if (height <= infos->actual_height){
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
    int blockfile;
    char dir[256];
    char temp[1024];
    size_t block_height;
    size_t bc_size;
    ssize_t r = read(fd, &block_height, sizeof(size_t));
    if (r != sizeof(size_t))
        return -1;
    r = read(fd, &bc_size, sizeof(size_t));
    if (r != sizeof(size_t))
        return -1;
    snprintf(dir, 256, "blockchain/c%iblock%lu", fd, block_height);

    blockfile = open(dir, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (blockfile == -1)
    {
        CLIENTMSG
        printf("Can't create c%iblock%lu file\n", fd, block_height);
        return -1;
    }

    while ((r = read(fd, temp, bc_size > 1024 ? 1024 : bc_size)) != 0 && bc_size > 0)
    {
        if (r == -1)
            errx(EXIT_FAILURE, "Can't read block %lu in connection fd: %i", block_height, fd);
        safe_write(blockfile, temp, r);
        bc_size -= r;
    }
    if (bc_size > 0){
        WARNINGMSG("Failed to read all the block!\n")
    }
    
    CLIENTMSG
    printf("Recived block %lu in connection fd: %i\n", block_height, fd);

    // LOAD BLOCK
    Block *block = malloc(sizeof(Block));
    lseek(blockfile, 0, SEEK_SET);
    convert_data_to_block(block, blockfile);
    close(blockfile);

    // ADD TO BLOCKCHAIN IF TRUE
    if (plebe_verify_block(block) == 0) {
        snprintf(temp, 256, "blockchain/block%lu", block_height);
        int ret = rename(dir, temp);
	
        if(ret == 0) {
            CLIENTMSG
            printf("File renamed successfully\n");
        } else {
            CLIENTMSG
            printf("Error: unable to rename the file\n");
        }
    }
    else
    {
        int ret = remove(dir);
        if(ret == 0) {
            CLIENTMSG
            printf("File remove successfully");
        } else {
            CLIENTMSG
            printf("Error: unable to remove the file");
        }
    }
    
    free_block(block);
    return 0;
}

int read_vote(__attribute__((unused))int fd){
    
    return 0;
}

int read_epoch_block(int fd){
    Block my_new_epoch;
    convert_data_to_block(&my_new_epoch, fd);
    // GESTION EPOCH
    return 0;
}

int read_send_pending_transaction_list(int fd, infos_st *infos){
    size_t nbtxids = 0;
    read(fd, &nbtxids, sizeof(size_t));
    time_t txids[500];
    for (size_t i = 0; i < nbtxids; i++)
    {
        read(fd, txids + i, sizeof(time_t) * nbtxids);
    }
    for (size_t i = 0; i < nbtxids; i++)
    {
        char temp[50];
        sprintf(temp, "./pdt/%ld", *(txids + i));
        if( access( temp, F_OK ) != 0 ) {
            // file doesn't exists
            send_get_pending_transaction(fd, *(txids + i));
            read_header(fd, infos);
        }
    }
    return nbtxids;
}

int read_send_pending_transaction(int fd){
    char dir[256];
    char temp[1024];
    size_t bc_size;
    time_t txid;
    ssize_t r = read(fd, &txid, sizeof(time_t));
    if (r != sizeof(time_t))
        return -1;
    r = read(fd, &bc_size, sizeof(size_t));
    if (r != sizeof(size_t))
        return -1;
    
    snprintf(dir, 256, "pdt/%ld", txid);
    if( access( dir, F_OK ) == 0 ) {
        // file exists
        return txid;
    }

    int transfile = open(dir, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (transfile == -1)
    {
        CLIENTMSG
        printf("Can't create transaction %ld file\n", txid);
        return -1;
    }

    while ((r = read(fd, temp, bc_size > 1024 ? 1024 : bc_size)) != 0 && bc_size > 0)
    {
        if (r == -1)
            errx(EXIT_FAILURE, "Can't read transaction %ld in connection fd: %i", txid, fd);
        safe_write(transfile, temp, r);
        bc_size -= r;
    }
    if (bc_size > 0){
        WARNINGMSG("Failed to read all the block!")
    }
    close(transfile);
    SERVERMSG
    printf("Recived read_pending_transaction %lu \
    transaction in connection fd: %i\n", txid, fd);
    return txid;
}

int read_get_pending_transaction(int fd){
    
    time_t txid;
    read(fd, &txid, sizeof(time_t));
    send_send_pending_transaction(fd, txid);
    return txid;
}
