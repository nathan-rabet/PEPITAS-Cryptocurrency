#include "network/network.h"

void send_client_list(int sockfd)
{
    Node *client_list = get_my_node();

    safe_write(sockfd, HD_SEND_CLIENT_LIST, strlen(HD_SEND_CLIENT_LIST));

    for (size_t index = 0; index < MAX_NEIGHBOURS; index++)
    {
        if (client_list->neighbours[index].hostname != NULL)
        {
            safe_write(sockfd, (void *)&client_list->neighbours[index].family, sizeof(int));

            int hostname_size;

            // IPv4
            if (client_list->neighbours[index].family == INET_ADDRSTRLEN)
                hostname_size = 15;

            // IPv6
            else
                hostname_size = 39;
            safe_write(sockfd, (void *)client_list->neighbours[index].hostname, hostname_size);
        }
    }

    // END SENDING
    safe_write(sockfd, "\r\n\r\n", 5);
}