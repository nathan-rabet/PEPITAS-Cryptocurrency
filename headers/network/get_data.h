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
 * @param infos Shared information
 * @return 0 if sucess, -1 otherwise
 */
size_t read_header(int sockfd, infos_st *infos);

/**
 * @brief Fetches the client list from a socket fd
 * 
 * @param who Tells if it is the server or the client side
 * @param fd The socket fd
 * 
 * @return 0 if sucess, -1 otherwise
 */
int fetch_client_list(char who, int fd);

/**
 * @brief Read blocks from a sock fd
 * 
 * @param fd The sock fd
 * @param infos Shared information
 * @return int 
 */
int read_get_blocks(int fd, infos_st *infos);

/**
 * @brief Get the actual height of a node via its sock fd
 * 
 * @param fd The sock fd
 * @return size_t 
 */
size_t read_actual_height(int fd);

/**
 * @brief Read a socket sended block
 * 
 * @param fd The socket fd
 * @return int 
 */
int read_send_block(int fd);

/**
 * @brief Read a socket sended vote
 * 
 * @param fd The socket fd
 * @param infos Shared information
 * @return int
 */
int read_vote(int fd, infos_st* infos);

/**
 * @brief Read a socket sended epoch block
 * 
 * @param fd The socket fd
 * @return int
 */
int read_epoch_block(int fd);

/**
 * @brief Get a socket sended pending transaction
 * 
 * @param fd The socket fd
 * @return int
 */
int read_get_pending_transaction(int fd);

/**
 * @brief Read a socket sended pending transaction
 * 
 * @param fd The socket fd
 * @param infos Shared information
 * @return int
 */
int read_send_pending_transaction(int fd, infos_st* infos);

/**
 * @brief Read a socket sended pending transaction list
 * 
 * @param fd The socket fd
 * @param infos Shared information
 * @return int
 */
int read_send_pending_transaction_list(int fd, infos_st *infos);

/**
 * @brief Epoch validation protocol
 * 
 * @param blockfile The epoch FD
 * @param height The epoch height
 * @param id The epoch ID
 * @return int 
 */
int epoch_validation_process(int blockfile, size_t height, int id);

#endif