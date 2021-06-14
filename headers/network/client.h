#ifndef CLIENT_H
#define CLIENT_H

#include "network/network.h"
#include "network/server.h"
#include "network/get_data.h"
#include "network/send_data.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <semaphore.h>
#include <stddef.h>

/**
 * @brief Get the my node object
 * 
 * @param who Tells if it is the server or the client side
 * @return Node* 
 */
Node *get_my_node(char who);

/**
 * @brief Sets a neighbour in the client.neightbours section
 * 
 * @param who Tells if it is the server or the client side
 * @param hostname The neighbour IP adress
 * @param family The type of IP adress
 * @return 0 if sucess, -1 otherwise if full
 */
int set_neighbour(char who, char *hostname, int family);

/**
 * @brief Remove a neighbour in the client.neightbours section
 * 
 * @param who Tells if it is the server or the client side
 * @param index The index of the neigbour to remove in client.neightbours
 */
void remove_neighbour(char who, int index);

/**
 * @brief Return the nb of neighbour in the client.neightbours section
 * 
 * @param who Tells if it is the server or the client side
 */
int number_neighbours(char who);

/**
 * @brief Print neighbours list 
 * 
 * @param who Tells if it is the server or the client side
 * @param mask 
 */
void print_neighbours(char who, char mask);

/**
 * @brief Save neighbours list in .neighbours/neighbours
 * 
 * @param who Tells if it is the server or the client side
 */
void save_neighbours(char who);

/**
 * @brief Load neighbours list from .neighbours/neighbours
 * 
 * @param who Tells if it is the server or the client side
 */
void load_neighbours(char who);

/**
 * @brief Tries to connect to the peer-to-peer network 
 * via a node in the Node structure
 * 
 * @param infos Some shared information
 * @param neighbour The neighbour to connect with
 * @param connection_type The type of connection
 * @param connection The connection* structure
 * @return socket FD or -1 if an error occurs
 */
connection *listen_to(infos_st *infos, Neighbour neighbour, char *connection_type, connection *connection);

/**
 * @brief Find if connection has any empty field
 * 
 * @param max The number of maximum connections
 * @param connection The connection* buffer 
 * @return int 
 */
int find_empty_connection(int max, connection *connection);

/**
 * @brief Check if `hostname` is in `client.neightbours`
 * 
 * @param who Tells if it is the server or the client side
 * @param hostname The IP adress to check
 * @return int 
 */
int is_in_neighbours(char who, char *hostname);

/**
 * @brief Create a client thread
 * 
 * @param args
 * @return void* 
 */
void *client_thread(void *args);

#endif