#ifndef GET_DATA_H
#define GET_DATA_H

#include <string.h>

#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "validation/validation_engine.h"
#include "ui/ui.h"

/**
 * @brief Waits a header in 'sockfd', reads it and processes it
 * 
 * @param sockfd The sock FD
 * @return 0 if sucess, -1 otherwise
 */
size_t read_header(int sockfd, infos_st *infos);

/**
 * @brief Merges my neighbours list with the one sent by 'neighbour_id'. 
 * 
 * @param sockfd The sockfd to read
 * 
 * @return 0 if sucess, -1 otherwise
 */
int fetch_client_list(char who, int fd);

int read_get_blocks(int fd, infos_st *infos);

size_t read_actual_height(int fd);
int read_send_block(int fd);
int read_vote(int fd, infos_st* infos);
int read_epoch_block(int fd, infos_st* infos);
int read_get_pending_transaction(int fd);
int read_send_pending_transaction(int fd, infos_st* infos);
int read_send_pending_transaction_list(int fd, infos_st *infos);

#endif