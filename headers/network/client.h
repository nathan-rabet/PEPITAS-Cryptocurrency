#include <stddef.h>

#ifndef CLIENT_H
#define CLIENT_H

#define MAX_NEIGHBOURS 64

typedef struct Neighbour
{
    int family;     // Use AF_* for IPv4, IPv6 or other addrinfo fields
    char *hostname; // The adress of the neighbours; NULL if free
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
Node* get_my_node(char who);

/**
 * @brief Sets a neighbour in the client.neightbours section
 * 
 * @return 0 if sucess, -1 otherwise if full
 */
int set_neighbour(char who, char *hostname, int family);

/**
 * @brief Remove a neighbour in the client.neightbours section
 */
void remove_neighbour(char who, int index);

/**
 * @brief return the nb of neighbour in the client.neightbours section
 */
int number_neighbours(char who);

/**
 * @brief Print neighbours list
 */
void print_neighbours(char who, char mask);

/**
 * @brief Save neighbours list in .neighbours/neighbours
 */
void save_neighbours(char who);

/**
 * @brief Load neighbours list from .neighbours/neighbours
 */
void load_neighbours(char who);

/**
 * @brief Tries to connect to the peer-to-peer network 
 * via a node in the Node structure
 * 
 * @param neighbour The neighbour to connect with
 * @return socket FD or -1 if an error occurs
 */
int listen_to(Neighbour neighbour);

int find_empty_connection(int max);

void *client_thread(void *args);

#endif