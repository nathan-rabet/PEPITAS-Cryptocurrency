#include "network/send_data.h"

int send_client_list(char who, int sockfd, char *sockip)
{
    Node *client_list = get_my_node(who);

    if (safe_write(sockfd, HD_SEND_CLIENT_LIST, strlen(HD_SEND_CLIENT_LIST)-1) == -1)
        return -1;

    int ng;
    for (size_t index = 0; index < MAX_NEIGHBOURS; index++)
        if (client_list->neighbours[index].hostname != NULL)
            ng++;

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
}

void send_get_blocks(client_connection *cc){
    safe_write(cc->clientfd, HD_GET_BLOCKS, sizeof(HD_GET_BLOCKS));
    safe_write(cc->clientfd, cc->Payload, cc->Playloadsize);
}
void send_actual_height(int fd, infos_st *infos){
    safe_write(fd, HD_ACTUAL_HEIGHT, sizeof(HD_ACTUAL_HEIGHT));
    safe_write(fd, &infos->actual_height, sizeof(size_t));
}

void send_reject_demand(int fd){
    safe_write(fd, HD_REJECT_DEMAND, sizeof(HD_REJECT_DEMAND));
}

void send_send_block(int fd, size_t height){
    safe_write(fd, HD_SEND_BLOCK, sizeof(HD_SEND_BLOCK));
    Block *block = get_block(height);
    write_block(*block, fd);
    free_block(block);
}

void send_pending_transaction_list(int fd){

}
