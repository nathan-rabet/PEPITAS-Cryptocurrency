#include "network/client.h"
#include "network/server.h"
#include "network/network.h"

Node *get_my_node()
{
    static Node node = {0};
    if (node.neighbours == NULL)
        node.neighbours = calloc(MAX_NEIGHBOURS, sizeof(Neighbour));

    return &node;
}

int set_neighbour(char *hostname, int family)
{
    Node *node = get_my_node();

    if (hostname == NULL)
    {
        size_t index = 0;
        while (node->neighbours[index].hostname != NULL && index < MAX_NEIGHBOURS)
            index++;
        if (index < MAX_NEIGHBOURS && node->neighbours[index].hostname == NULL)
        {
            for (size_t i = 0; i < NB_HARD_CODED_ADDR; i++)
            {
                node->neighbours[index].hostname = HARD_CODED_ADDR[i].hostname;
                node->neighbours[index].family = HARD_CODED_ADDR[i].family;

                if (listen_to(index) == -1)
                    memset(&node->neighbours[index], 0, sizeof(Neighbour));
                else
                    return 0;
            }
        }

        return -1;
    }

    else
    {
        size_t index = 0;
        while (index < MAX_NEIGHBOURS)
        {
            if (node->neighbours[index].hostname == NULL)
            {
                node->neighbours[index].hostname = hostname;
                node->neighbours[index].family = family;
                return listen_to(index);
            }
            index++;
        }
        return -1;
    }
}

int listen_to(size_t neighbour_id)
{
    struct addrinfo hints = {0};
    Neighbour neighbour = get_my_node()->neighbours[neighbour_id];

    hints.ai_family = neighbour.family; //IPV4 or maybe IPV6
    hints.ai_socktype = SOCK_STREAM;    //TCP

    struct addrinfo *result;
    int addrinfo_ret;

    // If no neighbour
    if (neighbour.hostname == NULL)
        return -1;

    // Get adress information
    addrinfo_ret = getaddrinfo(neighbour.hostname, STATIC_PORT, &hints, &result);

    // If adress information fetching failed
    if (addrinfo_ret != 0)
    {
        dprintf(STDERR_FILENO, "Fail getting information for address '%s' on port %s: %s",
                neighbour.hostname, STATIC_PORT, gai_strerror(addrinfo_ret));
        return -1;
    }

    // Try to connect for each result
    int sockfd;
    struct addrinfo *rp; // result points to a linked list
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd != -1)
        {
            // Try to connect
            if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
                break;
            // Fail to connect
            close(sockfd);
        }
    }

    freeaddrinfo(result);

    if (rp != NULL)
    {
        // Connection success
        get_my_node()->neighbours[neighbour_id].client_sockfd = sockfd;
        return 0;
    }

    // Connection failed
    return -1;
}