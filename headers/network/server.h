#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include "network.h"
#include "core/blockchain/block.h"

typedef struct client_connection
{
    struct addrinfo info; // Adress information
    int socket; // The client<->server connection socket
} client_connection;


/**
 * @brief Launches a server instance,
 * connected to the peer-to-peer network 'hostname'
 * 
 * @return 0 if success, -1 otherwise
 */
int init_server();


/**
 * @brief Sends a block to a user via a socket FD
 * 
 * @param sockfd The socket FD
 * @param block The block to send
 * @return int 
 */
int send_block(Block block,int sockfd);

#endif