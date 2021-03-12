#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>

/**
 * @brief Launch a server instance,
 * connected to the peer-to-peer network 'hostname'
 * 
 * @return 0 if sucess, -1 if not
 */
int init_server();

/**
 * @brief Tell to the client that the server is awake
 * 
 * @param sockfd The socket FD
 * 
 * @return int
 */
int server_im_awake(int sockfd);

#endif