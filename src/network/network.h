#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <err.h>
#include <string.h>

#define USER_LIST_SIZE 100
#define STATIC_DNS "localhost"
#define SERVER_PORT "12345"
#define CLIENT_PORT "54321"
#define BUF_SIZE 256

typedef struct 
{
    int family; // Use AF_* for IPv4, IPv6 or other addrinfo fields
    char* hostname; // The adress of the client
} ClientData;

/**
 * @brief Try to connect to the peer-to-peer network via a node in client_list
 * 
 * @param client_list List of potential client to connect with
 * @return socket FD or -1 if error
 */
int connect_to_network(ClientData client_list[]);


/**
 * @brief Get the client data object from a socket
 * 
 * @param sockfd The socket FD
 * @return ClientData 
 */
ClientData get_client_data(int sockfd);

/**
 * @brief Launch a server instance, 
 * connected to the peer-to-peer network 'hostname'
 * 
 * @param hostname
 * @return int 
 */
int init_server(char* hostname);

#endif