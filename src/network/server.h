#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include "network.h"

typedef struct client_connection
{
    struct addrinfo info; // Adress information
    int socket; // The client<->server connection socket
} client_connection;


/**
 * @brief Launch a server instance,
 * connected to the peer-to-peer network 'hostname'
 * 
 * @return 0 if sucess, -1 if not
 */
int init_server();

void send_client_list(int sockfd);

#endif