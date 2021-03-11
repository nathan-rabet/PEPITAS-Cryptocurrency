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

#define MAX_NEIGHBOURS_NB 64
#define STATIC_DNS "localhost"
#define STATIC_PORT "8080"
#define BUF_SIZE 256

#define GET_CLIENT "GET CLIENT\r\n\r\n"

typedef struct Neighbour
{
    int family;     // Use AF_* for IPv4, IPv6 or other addrinfo fields
    char *hostname; // The adress of the neighbours; NULL if free
} Neighbour;

typedef struct Client
{
    Neighbour *neighbours; // Neighbours list
} Client;

typedef struct Connection_state
{
    char is_connected;
    int sockfd;
} Connection_state;

Client* get_client();

/**
 * @brief Sets some neighbours in the client.neightbours section
 * 
 * @return 0 if sucess, -1 if not 
 */
int set_neighbours();

/**
 * @brief Try to connect to the peer-to-peer network 
 * via a node in the Client structure
 * 
 * @return socket FD or -1 if error
 */
int connect_to_network(int client_to_connect_id);

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