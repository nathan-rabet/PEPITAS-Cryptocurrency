#ifndef CLIENT_H
#define CLIENT_H

#include "network.h"

#define MAX_NEIGHBOURS 64

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
int set_neighbours(char *hostname, int family);

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
void wait_header(int sockfd);


void read_header(char *buf, int sockfd);

void recive_client_list(int sockfd);

#endif