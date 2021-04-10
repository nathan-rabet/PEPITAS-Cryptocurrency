#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "network.h"
#include "core/blockchain/block.h"

#define NODESERVER 0
#define DOORSERVER 1

typedef struct client_connection
{
    struct addrinfo *info; // Adress information
    int socket; // The client<->server connection socket
} client_connection;


/**
 * @brief Launches a server instance,
 * connected to the peer-to-peer network 'hostname'
 * 
 * @param type Type of the server
 * @return 0 if success, -1 otherwise
 */
int init_server(char type);

#endif