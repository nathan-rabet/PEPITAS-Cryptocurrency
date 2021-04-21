#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <netdb.h>
#include "network.h"
#include "core/blockchain/block.h"

#define NODESERVER 0
#define DOORSERVER 1

#define MAX_SERVER 20

typedef struct client_connection
{
    pthread_t thread; // Adress information
    sem_t lock;
    int demand;
    int clientfd; // The client<->server connection socket
    size_t Playloadsize;
    void *Payload;
} client_connection;


/**
 * @brief Launches a server instance,
 * connected to the peer-to-peer network 'hostname'
 * 
 * @param type Type of the server
 * @return 0 if success, -1 otherwise
 */
void *init_server(void *args);

#endif