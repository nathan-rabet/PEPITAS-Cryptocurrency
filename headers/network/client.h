#include <stddef.h>

#ifndef CLIENT_H
#define CLIENT_H

#define MAX_NEIGHBOURS 64

typedef struct Neighbour
{
    int family;     // Use AF_* for IPv4, IPv6 or other addrinfo fields
    char *hostname; // The adress of the neighbours; NULL if free
    int server_sockfd; // The socket to SEND data
    int client_sockfd; // The socket to GET data
} Neighbour;

typedef struct Node
{
    Neighbour *neighbours; // Neighbours list
} Node;

/**
 * @brief Get the my node object
 * 
 * @return Node* 
 */
Node* get_my_node();

/**
 * @brief Sets a neighbour in the client.neightbours section
 * 
 * @return 0 if sucess, -1 if not 
 */
int set_neighbour(char *hostname, int family);

/**
 * @brief Try to connect to the peer-to-peer network 
 * via a node in the Node structure
 * 
 * @param neighbour_id The neighbour index (in struct Node) to connect with
 * @return socket FD or -1 if error
 */
int listen_to(size_t neighbour_id);

/**
 * @brief Ping the client side of 'neighbour_id'
 * and delete it from struct Node if no response
 * 
 * @param neighbour_id 
 * @return 0 if sucess, -1 if not
 */
int ping_client(size_t neighbour_id);

#endif