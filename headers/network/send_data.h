#ifndef SEND_DATA_H
#define SEND_DATA_H

#include "network/server.h"
#include <dirent.h>
#include <stdio.h>

/**
 * @brief Sends my client list to a node via 'sockfd'
 * 
 * @param sockfd The sock FD
 * @return 0 if success, -1 otherwise
 */
int send_client_list(char who, int sockfd, char *sockip);

/**
 * @brief Sends get blocks
 * 
 */
void send_get_blocks(client_connection *cc);

void send_actual_height(int fd, infos_st *infos);

void send_reject_demand(int fd);

void send_send_block(int fd, size_t height);

void send_pending_transaction_list(int fd);

void send_send_pending_transaction(int fd, time_t txid);

void send_get_pending_transaction(int fd, time_t txid);

void send_epoch_block(client_connection *cc);

void send_vote_fd(client_connection *cc);

#endif