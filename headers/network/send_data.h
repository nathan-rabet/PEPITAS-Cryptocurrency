#ifndef SEND_DATA_H
#define SEND_DATA_H

#include "network/server.h"

/**
 * @brief Sends my client list to a node via 'sockfd'
 * 
 * @param sockfd The sock FD
 * @return 0 if success, -1 otherwise
 */
int send_client_list(char who, int sockfd, char *sockip);

/**
 * @brief Sends get blocks header
 * 
 */
void send_get_blocks(client_connection *cc);

void send_inv()

#endif