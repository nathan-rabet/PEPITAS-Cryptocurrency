#ifndef NETWORK_H
#define NETWORK_H


#include <pthread.h>
#include <semaphore.h>

#define SIZE_OF_HOSTNAME 39
#define NB_HARD_CODED_ADDR 2
#define MAX_CONNECTION 5

#define STATIC_PORT "4242"

#define P_VERSION 0

#define IM_SERVER 0
#define IM_CLIENT 1

#define MAX_NEIGHBOURS 64

#define NODESERVER 0
#define DOORSERVER 1

#define MAX_SERVER 20

typedef struct Neighbour
{
    int family;     // Use AF_* for IPv4, IPv6 or other addrinfo fields
    char *hostname; // The adress of the neighbours; NULL if free
} Neighbour;

typedef struct Node
{
    Neighbour *neighbours; // Neighbours list
} Node;

typedef struct client_connection
{
    pthread_t thread; // Adress information
    sem_t lock;
    int demand;
    int clientfd; // The client<->server connection socket
    size_t Playloadsize;
    void *Payload;
} client_connection;

typedef struct infos_st
{
    char is_sychronize;
    size_t actual_height;
    char serv_type;
}infos_st;
typedef struct th_arg
{
    infos_st *infos;
    client_connection *client_con;
}th_arg;

extern const Neighbour HARD_CODED_ADDR[];

// REQUESTS
#define HD_GET_CLIENT_LIST                  "GET CLIENT LIST\r\n\r\n"
#define HD_SEND_CLIENT_LIST                 "SEND CLIENT LIST\n"
#define HD_CONNECTION_TO_NETWORK            "CONNECTION TO NETWORK\r\n\r\n"
#define HD_CONNECTION_TO_NODE               "CONNECTION TO NODE\r\n\r\n"
#define HD_GET_BLOCKS                       "GET BLOCKS\n"
#define HD_ACTUAL_HEIGHT                    "ACTUAL HEIGHT\n"
#define HD_SEND_BLOCK                       "SEND BLOCK\n"
#define HD_GET_PENDING_TRANSACTION_LIST     "GET PENDING TRANSACTION LIST\r\n\r\n"
#define HD_REJECT_DEMAND                    "REJECT DEMAND\r\n\r\n"
#define HD_SEND_PENDING_TRANSACTION         "SEND PENDING TRANSACTION\n"
#define HD_SEND_EPOCH_BLOCK                 "SEND EPOCH BLOCK\n"
#define HD_SEND_VOTE                        "SEND VOTE\n"

// DEMAND
#define DD_GET_BLOCKS                  1
#define DD_GET_BLOCKS_HEADERS          2

#endif