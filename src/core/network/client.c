#include "network/network.h"
#include "network/client.h"

client_connection *client_connections = NULL;

Node *get_my_node(char who)
{
    static Node node_server = {0};
    static Node node_client = {0};
    if (node_server.neighbours == NULL)
        node_server.neighbours = calloc(MAX_NEIGHBOURS, sizeof(Neighbour));
    if (node_client.neighbours == NULL)
        node_client.neighbours = calloc(MAX_NEIGHBOURS, sizeof(Neighbour));
    if (who == IM_SERVER)
        return &node_server;
    return &node_client;
}

int set_neighbour(char who, char *hostname, int family)
{
    Node *node = get_my_node(who);
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

void remove_neighbour(char who, int index)
{
    Node *node = get_my_node(who);
    if (index < MAX_NEIGHBOURS && index >= 0)
    {
        if (node->neighbours[index].hostname == NULL)
            free(node->neighbours[index].hostname);
        node->neighbours[index].family = 0;
    }
}

void print_neighbours(char who, char mask)
{

    Node *node = get_my_node(who);
    CLIENTMSG
    printf("Neighbour list:\n");
    for (size_t i = 0; i < MAX_NEIGHBOURS; i++)
    {
        if (!(!mask && node->neighbours[i].hostname == NULL))
        {
            CLIENTMSG
            printf("%02lu: hostname \"%s\", family \"%i\"\n", i, node->neighbours[i].hostname, node->neighbours[i].family);
        }
    }
}

void save_neighbours(char who)
{
    Node *node = get_my_node(who);

    struct stat st = {0};

    if (stat(".neighbours", &st) == -1)
    {
        mkdir(".neighbours", 0700);
    }

    FILE *nfile;
    if (who == IM_CLIENT)
    {
        nfile = fopen("./.neighbours/client_neighbours", "wb");
    }
    else
    {
        nfile = fopen("./.neighbours/server_neighbours", "wb");
    }
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

void load_neighbours(char who)
{
    Node *node = get_my_node(who);
    FILE *nfile;
    if (who == IM_CLIENT)
    {
        if (access(".neighbours/client_neighbours", F_OK))
        {
            return;
        }
        nfile = fopen("./.neighbours/client_neighbours", "rb");
    }
    else
    {
        if (access(".neighbours/server_neighbours", F_OK))
        {
            return;
        }
        nfile = fopen("./.neighbours/server_neighbours", "rb");
    }
    if (!nfile)
        err(errno, "Impossible to write '.neighbours/neighbours'");
    char temp[SIZE_OF_HOSTNAME + sizeof(int)];
    for (size_t i = 0; i < MAX_NEIGHBOURS; i++)
    {
        fread(temp, SIZE_OF_HOSTNAME + sizeof(int), 1, nfile);
        if (strncmp(temp, "\0\0\0\0\0\0\0\0\0\0", 10))
        {
            node->neighbours[i].hostname = malloc(SIZE_OF_HOSTNAME);
            memcpy(node->neighbours[i].hostname, temp, SIZE_OF_HOSTNAME);
            node->neighbours[i].family = *(int *)(temp + SIZE_OF_HOSTNAME);
        }
    }
    fclose(nfile);
}

int number_neighbours(char who)
{
    int nb_neigbours = 0;
    Node *node = get_my_node(who);
    for (size_t i = 0; i < MAX_NEIGHBOURS; i++)
    {
        if (node->neighbours[i].hostname != NULL)
            nb_neigbours++;
    }
    return nb_neigbours;
}

client_connection *listen_to(infos_st *infos, Neighbour neighbour, char *connection_type)
{
    struct addrinfo hints = {0};

    hints.ai_family = neighbour.family; //IPV4 or maybe IPV6
    hints.ai_socktype = SOCK_STREAM;    //TCP

    struct addrinfo *result;
    int addrinfo_ret;

    // If no neighbour
    if (neighbour.hostname == NULL)
        return NULL;

    // Get adress information
    addrinfo_ret = getaddrinfo(neighbour.hostname, STATIC_PORT, &hints, &result);

    // If adress information fetching failed
    if (addrinfo_ret != 0)
    {
        dprintf(STDERR_FILENO, "Fail getting information for address '%s' on port %s: %s",
                neighbour.hostname, STATIC_PORT, gai_strerror(addrinfo_ret));
        return NULL;
    }

    // Try to connect for each result
    int sockfd;
    struct addrinfo *rp;     // result points to a linked list
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sockfd == -1)
            continue; // The socket is not created

        #ifdef TEST
        // Set timeout for debug
        int tcp_timeout = 5000; // user timeout in milliseconds [ms]
        setsockopt(sockfd, SOL_TCP, TCP_USER_TIMEOUT, (char *)&tcp_timeout, sizeof(tcp_timeout));
        #endif

        // Try to connect
        if (connect(sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;
        // Fail to connect
        close(sockfd);
    }

    freeaddrinfo(result);

    if (rp != NULL)
    {
        // Connection success

        //SEND ACCEPT
        safe_write(sockfd, connection_type, strlen(connection_type));

        int index = find_empty_connection(MAX_CONNECTION);
        if (index != -1)
        {
            client_connections[index].clientfd = sockfd;
            client_connections[index].demand = 0;

            th_arg *args = malloc(sizeof(th_arg));
            args->infos = infos;
            args->client_con = &client_connections[index];

            pthread_create(&client_connections[index].thread, NULL, client_thread, args);

            return &client_connections[index];
        }
    }

    // Connection failed
    return NULL;
}

int find_empty_connection(int max)
{
    for (int i = 0; i < max; i++)
    {
        if (client_connections[i].clientfd == 0)
            return i;
    }
    return -1;
}

void *client_thread(void *args)
{
    th_arg *a = (th_arg *)args;
    client_connection *cc = a->client_con;
    infos_st *infos = a->infos;
    free(args);

    while (1)
    {
        // Wait demand
        sem_wait(&cc->lock);
        switch (cc->demand)
        {
        case DD_GET_HEIGHT:
            send_get_blocks(cc);
            cc->actual_client_height = read_header(cc->clientfd, infos);
            break;

        case DD_GET_BLOCKS:
        {
            send_get_blocks(cc);
            for (char i = 0; i < ((get_blocks_t *)cc->Payload)->nb_demands; i++)
            {
                read_header(cc->clientfd, infos);
                update_sync(infos->actual_height, cc->actual_client_height);
            }
            break;
        }

        case DD_SEND_TRANSACTION:
        {
            send_pending_transaction(cc->clientfd, *(time_t *)cc->Payload);
            break;
        }

        default:
            break;
        }
        cc->demand = 0;
    }
}