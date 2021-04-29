#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <semaphore.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "core/blockchain/block.h"
#include "network/client.h"
#include "network/get_data.h"
#include "network/send_data.h"
#include "network/network.h"
#include "misc/safe.h"


/**
 * @brief Launches a server instance,
 * connected to the peer-to-peer network 'hostname'
 * 
 * @param type Type of the server
 * @return 0 if success, -1 otherwise
 */
void *init_server(void *args);

#endif