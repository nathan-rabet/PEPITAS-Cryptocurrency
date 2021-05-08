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
    SERVERMSG
    printf("Send HD_SEND_BLOCK\n");
    // TEST IF BLOCK
    int blockfile;
    char dir[256];
    char temp[1024];
    ssize_t r = 0;

    snprintf(dir, 256, "blockchain/block%lu", height);
    struct stat st = {0};
    if (stat(dir, &st) == -1)
    {
        return;
    }

    blockfile = open(dir, O_RDONLY);
    if (blockfile == -1)
        return;
    safe_write(fd, HD_SEND_BLOCK, strlen(HD_SEND_BLOCK));
    safe_send(fd, (void *)&height, sizeof(size_t));
    while ((r = read(blockfile, temp, 1024) != 0))
    {
        if (r == -1)
            errx(EXIT_FAILURE, "Can't send block %lu\n", height);
        safe_send(fd, temp, r);
    }
}

void send_pending_transaction_list(__attribute__((unused))int fd){

}
