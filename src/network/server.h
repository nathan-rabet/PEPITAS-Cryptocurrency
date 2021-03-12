#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
typedef struct client_connection
{
    struct addrinfo info; // Adress information
    int socket; // The client<->server connection socket
} client_connection;


/**
 * @brief Launch a server instance,
 * connected to the peer-to-peer network 'hostname'
 * 
 * @return 0 if sucess, -1 if not
 */
int init_server();


/**
 * @brief Read a request header
 * 
 * @param sockfd The socket FD
 * @return char* 
 */
char * read_header(int sockfd);


/**
 * @brief Tell to the client that the server is awake
 * 
 * @param sockfd The socket FD
 * 
 * @return int
 */
int server_im_awake(int sockfd);

#endif