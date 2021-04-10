#include "network/network.h"

int send_client_list(int sockfd, char *sockip)
{
    Node *client_list = get_my_node();

    if (safe_write(sockfd, HD_SEND_CLIENT_LIST, strlen(HD_SEND_CLIENT_LIST)) == -1)
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

    // END SENDING
    return safe_write(sockfd, "\r\n\r\n", 5);
    
}