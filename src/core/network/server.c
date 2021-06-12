#include "network/server.h"

void *accept_connection(void *args)
{
    th_arg *a = (th_arg *)args;
    connection *server_connection = a->client_con;
    infos_st *infos = a->infos;
    free(args);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if (getpeername(server_connection->clientfd, (struct sockaddr * restrict) & client_addr, &client_addr_len) == -1)
        err(EXIT_FAILURE, "Failed to recover client IP address\n");

    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN) == NULL)
        err(EXIT_FAILURE, "Failed to convert client IP address to string\n");

    SERVERMSG
    printf("New connection: '%s'\n", ip_str);

    // VERIFY
    if (read_header(server_connection->clientfd, infos) == 1)
    {
        SERVERMSG
        printf("Accept connection: '%s'\n", ip_str);
    }
    else
    {
        close(server_connection->clientfd);
        server_connection->clientfd = 0;

        pthread_exit(NULL);
    }
    

    // CONNECTION BACK
    Node *node = get_my_node(IM_SERVER);
    if (is_in_neighbours(IM_CLIENT, ip_str) == -1) {
        int index = set_neighbour(IM_SERVER, ip_str, AF_INET);
        SERVERMSG
        printf("Connection back\n");
        if (node->neighbours[index].hostname != NULL)
        {
            if (listen_to(infos, node->neighbours[index], HD_CONNECTION_TO_NODE, client_connections) == NULL)
                printf("Fail de connection to neighbour\n");
        }
        
    }

    // SERVER ROUTINE
    while (1)
    {
        read_header(server_connection->clientfd, infos);
    }
    

    close(server_connection->clientfd);
    server_connection->clientfd = 0;

    pthread_exit(NULL);
}

void *redirect_connection(void *arg)
{
    int clientfd = *((int *)arg);

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    if (getpeername(clientfd, (struct sockaddr * restrict) & client_addr, &client_addr_len) == -1)
        err(EXIT_FAILURE, "Failed to recover client IP address\n");

    char ip_str[39];
    if (inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN) == NULL)
        err(EXIT_FAILURE, "Failed to convert client IP address to string\n");


    printf("New connection: '%s'\n", ip_str);

    if (read_header(clientfd, NULL) == 1)
    {
        printf("Accept connection: '%s'\n", ip_str);
        set_neighbour(IM_SERVER, ip_str, AF_INET);
        save_neighbours(IM_SERVER);
        print_neighbours(IM_SERVER, 0);
        //send client list
        send_client_list(IM_SERVER, clientfd, ip_str);
    }


    close(clientfd);
    free(arg);

    pthread_exit(NULL);
}

void *init_server(void *args)
{
    infos_st *infos = (infos_st *)args;
    connection *server_connections = calloc(MAX_SERVER, sizeof(connection));
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        sem_init(&server_connections[i].lock, 0, 0);
    }
    SERVERMSG
    printf("Opening client server...\n");

    struct addrinfo hints = {0}; //
    struct addrinfo *result;     //
    struct addrinfo *rp;         //
    int addrinfo_error;
    int sockfd; //File Descriptor of the socket

    hints.ai_family = AF_UNSPEC;     //IPV6 IPV4
    hints.ai_socktype = SOCK_STREAM; //TCP
    hints.ai_flags = AI_PASSIVE;     //Server

    // Get info
    addrinfo_error = getaddrinfo(NULL, STATIC_PORT, &hints, &result);

    // Error management
    if (addrinfo_error)
    {
        
        SERVERMSG
        printf("Fail getting address on port %s: %s\n",
             STATIC_PORT, gai_strerror(addrinfo_error));
        errx(EXIT_FAILURE, "Fail getting address on port %s: %s",
             STATIC_PORT, gai_strerror(addrinfo_error));
    }

    // result points to a linked list
    // try yo connect for each result
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sockfd = socket(rp->ai_family, rp->ai_socktype, 0);
        if (sockfd == -1)
            continue; // The socket is not created

        int chezpas = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &chezpas, sizeof(int));

        // Try to connect
        if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == -1)
        {
            close(sockfd);
            continue;
        }
        break;
    }


    if (rp == NULL)
    { /* No address succeeded */
        SERVERMSG
        printf("Could not bind\n");
        //exit(EXIT_FAILURE);
    }

    listen(sockfd, 5);

    while (1)
    {
        if (infos->serv_type == DOORSERVER)
        {
            pthread_t thread;
            int *clientfd = calloc(1, sizeof(int));
            *clientfd = accept(sockfd, rp->ai_addr, &rp->ai_addrlen);
            pthread_create(&thread, NULL, redirect_connection, clientfd);
        }
        
        if (infos->serv_type == NODESERVER)
        {
            int index = find_empty_connection(MAX_SERVER, server_connections);
            server_connections[index].clientfd = accept(sockfd, rp->ai_addr, &rp->ai_addrlen);
            if (server_connections[index].clientfd != -1)
            {
                th_arg *args = malloc(sizeof(th_arg));
                args->infos = infos;
                args->client_con = &server_connections[index];
                pthread_create(&server_connections->thread, NULL, accept_connection, args);
            }
            else
                server_connections[index].clientfd = 0;
        }
    }
}