#ifndef GET_DATA_H
#define GET_DATA_H

#include <string.h>

#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"

/**
 * @brief Waits a header in 'sockfd', reads it and processes it
 * 
 * @param sockfd The sock FD
 * @return 0 if sucess, -1 otherwise
 */
int read_header(int sockfd, infos_st *infos);

/**
 * @brief Merges my neighbours list with the one sent by 'neighbour_id'. 
 * 
 * @param sockfd The sockfd to read
 * 
 * @return 0 if sucess, -1 otherwise
 */
int fetch_client_list(char who, char *buffer, size_t buffer_size);

int read_get_blocks(char *header, int fd, infos_st *infos);

size_t read_actual_height(char *header);

int read_send_block(char *header, int fd);
int read_vote(char *header, int fd);
int read_pending_transaction_list(char *header, int fd);
int read_epoch_block(char *header, int fd);

#endif