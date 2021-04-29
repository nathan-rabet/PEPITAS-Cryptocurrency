#ifndef SERVER_H
#define SERVER_H

#include "network.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <netdb.h>
#include "core/blockchain/block.h"

/**
 * @brief Launches a server instance,
 * connected to the peer-to-peer network 'hostname'
 * 
 * @param type Type of the server
 * @return 0 if success, -1 otherwise
 */
void *init_server(void *args);

#endif