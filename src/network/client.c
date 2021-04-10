#include "network/client.h"
#include "network/server.h"
#include "network/network.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>

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
        ssize_t min_null = -1;
        while (index < MAX_NEIGHBOURS)
        {
            if (min_null == -1 && node->neighbours[index].hostname == NULL)
            {
                min_null = index;
            }
            if (node->neighbours[index].hostname != NULL && !strncmp(node->neighbours[index].hostname, hostname, strlen(hostname)))
            {
                return 0;
            }
            
            index++;
        }
        if (min_null != -1)
        {
            node->neighbours[min_null].hostname = malloc(sizeof(char) * 39);
            snprintf(node->neighbours[min_null].hostname, SIZE_OF_HOSTNAME, "%s", hostname);
            node->neighbours[min_null].family = family;
            return 0;
        }
        return -1;
    }
}

void remove_neighbour(int index)
{
    Node *node = get_my_node();
    if (index < MAX_NEIGHBOURS && index >= 0)
    {
        if (node->neighbours[index].hostname == NULL)
            free(node->neighbours[index].hostname);
        node->neighbours[index].family = 0;
    }
}

void print_neighbours()
{

    Node *node = get_my_node();
    printf("Neighbour list:\n");
    for (size_t i = 0; i < MAX_NEIGHBOURS; i++)
    {
        printf("%02lu: hostname \"%s\", family \"%i\"\n", i ,node->neighbours[i].hostname, node->neighbours[i].family);
    }

}

void save_neighbours()
{
    Node *node = get_my_node();

    struct stat st = {0};

    if (stat(".neighbours", &st) == -1)
    {
        mkdir(".neighbours", 0700);
    }

    FILE *nfile = fopen("./.neighbours/neighbours", "wb");
    if (!nfile)
        err(errno, "Impossible to write '.neighbours/neighbours'");
    char *temp = calloc(SIZE_OF_HOSTNAME + sizeof(int), 1);
    for (size_t i = 0; i < MAX_NEIGHBOURS; i++)
    {
        if (node->neighbours[i].hostname != NULL)
        {
            fwrite(node->neighbours[i].hostname, SIZE_OF_HOSTNAME, 1, nfile);
            fwrite((void *)&node->neighbours[i].family, sizeof(int), 1, nfile);
        }
        else
        {
            fwrite(temp, SIZE_OF_HOSTNAME + sizeof(int), 1, nfile);
        }
    }
    free(temp);
    fclose(nfile);
}

void load_neighbours()
{
    Node *node = get_my_node();
    if (access(".neighbours/neighbours", F_OK))
    {
        return;
    }
    FILE *nfile = fopen("./.neighbours/neighbours", "rb");
    if (!nfile)
        err(errno, "Impossible to write '.neighbours/neighbours'");
    char temp[SIZE_OF_HOSTNAME + sizeof(int)];
    for (size_t i = 0; i < MAX_NEIGHBOURS; i++)
    {
        fread(temp, SIZE_OF_HOSTNAME + sizeof(int), 1, nfile);
        if (strncmp(temp, "\0\0\0\0\0\0\0\0\0\0", 10))
        {
            node->neighbours[i].hostname = malloc(SIZE_OF_HOSTNAME);
            snprintf(node->neighbours[i].hostname, SIZE_OF_HOSTNAME, "%s", temp);
            node->neighbours[i].family = *(int *)(temp + SIZE_OF_HOSTNAME);
        }
    }
    fclose(nfile);
}

int listen_to(size_t neighbour_id)
{
    struct addrinfo *hints = {0};
    Neighbour neighbour = get_my_node()->neighbours[neighbour_id];

    hints->ai_family = neighbour.family; //IPV4 or maybe IPV6
    hints->ai_socktype = SOCK_STREAM;    //TCP

    struct addrinfo *result;
    int addrinfo_ret;

    // If no neighbour
    if (neighbour.hostname == NULL)
        return -1;

    // Get adress information
    addrinfo_ret = getaddrinfo(neighbour.hostname, STATIC_PORT, hints, &result);

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