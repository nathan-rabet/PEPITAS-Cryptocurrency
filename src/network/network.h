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

#define USER_LIST_SIZE 64
#define STATIC_DNS "localhost"
#define SERVER_PORT "12345"
#define CLIENT_PORT "54321"
#define BUF_SIZE 256

typedef struct Client
{
    Neighbours *neighbours; // Neighbours list
} Client;

static Client client = {0};

typedef struct Neighbours
{
    int family;     // Use AF_* for IPv4, IPv6 or other addrinfo fields
    char *hostname; // The adress of the neighbours
} Neighbours;

/**
 * @brief Try to connect to the peer-to-peer network 
 * via a node in the Client structure
 * 
 * @return socket FD or -1 if error
 */
int connect_to_network();

/**
 * @brief Set the client data object from a socket 
 * in the client->neighbours list
 * 
 * @param sockfd The socket FD
 */
void get_client_data(int sockfd);

/**
 * @brief Launch a server instance,
 * connected to the peer-to-peer network 'hostname'
 * 
 * @return 0 if sucess, -1 if not
 */
int init_server();

#endif