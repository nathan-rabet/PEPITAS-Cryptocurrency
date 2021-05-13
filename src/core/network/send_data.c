#include "network/send_data.h"

int send_client_list(char who, int sockfd, char *sockip)
{
    Node *client_list = get_my_node(who);

    if (safe_write(sockfd, HD_SEND_CLIENT_LIST, strlen(HD_SEND_CLIENT_LIST)) == -1)
        return -1;

    int ng = 0;
    for (size_t index = 0; index < MAX_NEIGHBOURS; index++)
    {
        int hostname_size;

        // IPv4
        if (client_list->neighbours[index].family == AF_INET)
            hostname_size = 15;

        // IPv6
        else
            hostname_size = 39;
        if (client_list->neighbours[index].hostname != NULL && strncmp(sockip, client_list->neighbours[index].hostname, hostname_size))
            ng++;
    }
    
    if (safe_write(sockfd, (void *)&ng, sizeof(int)) == -1)
        return -1;

    for (size_t index = 0; index < MAX_NEIGHBOURS; index++)
    {
        int hostname_size;

        // IPv4
        if (client_list->neighbours[index].family == AF_INET)
            hostname_size = 15;

        // IPv6
        else
            hostname_size = 39;
        if (client_list->neighbours[index].hostname != NULL && strncmp(sockip, client_list->neighbours[index].hostname, hostname_size))
        {
            if (safe_write(sockfd, (void *)&client_list->neighbours[index].family, sizeof(int)) == -1)
                return -1;

            if (safe_write(sockfd, (void *)client_list->neighbours[index].hostname, hostname_size) == -1)
                return -1;
        }
    }
    return 0;
}

void send_get_blocks(client_connection *cc){
    CLIENTMSG
    printf("Send HD_GET_BLOCKS\n");
    safe_write(cc->clientfd, HD_GET_BLOCKS, strlen(HD_GET_BLOCKS));
    safe_send(cc->clientfd, cc->Payload, cc->Playloadsize);
}
void send_actual_height(int fd, infos_st *infos){
    CLIENTMSG
    printf("Send HD_ACTUAL_HEIGHT\n");
    safe_write(fd, HD_ACTUAL_HEIGHT, strlen(HD_ACTUAL_HEIGHT));
    safe_write(fd, &infos->actual_height, sizeof(size_t));
}

void send_reject_demand(int fd){
    SERVERMSG
    printf("Send HD_REJECT_DEMAND\n");
    safe_write(fd, HD_REJECT_DEMAND, strlen(HD_REJECT_DEMAND));
}

void send_send_block(int fd, size_t height){
    
    char dir[256];
    char temp[1024];
    size_t bc_size = 0;
    ssize_t r = 0;

    snprintf(dir, 256, "blockchain/block%lu", height);

    FILE *blockfile = fopen(dir, "r");
    if (blockfile == NULL)
        return;
    fseek(blockfile, 0L, SEEK_END);
    bc_size = ftell(blockfile);
    fseek(blockfile, 0L, SEEK_SET);
    safe_write(fd, HD_SEND_BLOCK, strlen(HD_SEND_BLOCK));
    safe_send(fd, (void *)&height, sizeof(size_t));
    safe_send(fd, (void *)&bc_size, sizeof(size_t));
    while ((r = fread(temp, 1, 1024, blockfile)) != 0 && bc_size > 0)
    {
        if (r == -1)
            errx(EXIT_FAILURE, "Can't send block %lu\n", height);
        safe_write(fd, temp, r);
        bc_size -= r;
    }
    if (bc_size > 0){
        WARNINGMSG("Failed to send all the block!")
    }
    SERVERMSG
    printf("Send block %lu to fd %i\n", height, fd);
    fclose(blockfile);
}

void send_pending_transaction_list(int fd){
    CLIENTMSG
    printf("Send HD_SEND_PENDING_TRANSACTION_LIST\n");
    safe_write(fd, HD_SEND_PENDING_TRANSACTION_LIST, strlen(HD_SEND_PENDING_TRANSACTION_LIST));
    size_t nbdir = 0;
    time_t txids[500];
    DIR *d;
    struct dirent *dir;
    d = opendir("./pdt");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG)
            {
                sscanf((char *)(txids + nbdir), "%hhu", dir->d_name);
                nbdir++;
            }
        }
        closedir(d);
    }
    safe_send(fd, &nbdir, sizeof(size_t));
    safe_send(fd, txids, sizeof(time_t) * nbdir);
}

void send_send_pending_transaction(int fd, time_t txid){
    char dir[256];
    char temp[1024];
    size_t bc_size = 0;
    ssize_t r = 0;

    snprintf(dir, 256, "pdt/%ld", txid);
    FILE *transfile = fopen(dir, "r");
    if (transfile == NULL)
    {
        CLIENTMSG
        printf("Transaction not found :(\n");
        send_reject_demand(fd);
        return;
    }
    fseek(transfile, 0L, SEEK_END);
    bc_size = ftell(transfile);
    fseek(transfile, 0L, SEEK_SET);

    safe_write(fd, HD_SEND_PENDING_TRANSACTION, strlen(HD_SEND_PENDING_TRANSACTION));
    safe_send(fd, &txid, sizeof(time_t));
    safe_send(fd, &bc_size, sizeof(size_t));

    while ((r = fread(temp, 1, 1024, transfile)) != 0 && bc_size > 0)
    {
        if (r == -1)
            errx(EXIT_FAILURE, "Can't send transaction %ld\n", txid);
        safe_send(fd, temp, r);
        bc_size -= r;
    }
    if (bc_size > 0){
        WARNINGMSG("Failed to send all the transaction!");
    }
    fclose(transfile);
    CLIENTMSG
    printf("Send HD_SEND_PENDING_TRANSACTION with txid: %ld\n", txid);
}

void send_get_pending_transaction(int fd, time_t txid){

    safe_write(fd, HD_GET_PENDING_TRANSACTION, strlen(HD_GET_PENDING_TRANSACTION));
    safe_send(fd, &txid, sizeof(time_t));
    CLIENTMSG
    printf("Send HD_GET_PENDING_TRANSACTION with txid: %ld\n", txid);
}
