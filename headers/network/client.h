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
client_connection *listen_to(infos_st *infos, Neighbour neighbour);

int find_empty_connection(int max);

void *client_thread(void *args);

#endif