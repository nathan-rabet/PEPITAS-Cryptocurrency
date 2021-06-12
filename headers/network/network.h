#ifndef NETWORK_H
#define NETWORK_H


#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>

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

#define MAX_VALIDATORS_PER_BLOCK 512

#define SOL_TCP 6  // socket options TCP level
#define TCP_USER_TIMEOUT 18  // how long for loss retry before timeout [ms]


typedef struct Neighbour
{
    int family;     // Use AF_* for IPv4, IPv6 or other addrinfo fields
    char *hostname; // The adress of the neighbours; NULL if free
} Neighbour;

typedef struct Node
{
    Neighbour *neighbours; // Neighbours list
} Node;

typedef struct connection
{
    pthread_t thread; // Adress information
    sem_t lock;
    int demand;
    int clientfd; // The client<->server connection socket
    size_t Payloadsize;
    void *Payload;
    size_t actual_client_height;
} connection;

typedef struct infos_st
{
    char as_epoch;
    char is_validator;
    int validator_id;
    size_t actual_height;
    size_t pdt;
    char serv_type;
}infos_st;
typedef struct th_arg
{
    infos_st *infos;
    connection *client_con;
}th_arg;

extern const Neighbour HARD_CODED_ADDR[];

// REQUESTS
#define HD_GET_CLIENT_LIST                  "GET CLIENT LIST\r\n\r\n"
#define HD_SEND_CLIENT_LIST                 "SEND CLIENT LIST\r\n\r\n"
#define HD_CONNECTION_TO_NETWORK            "CONNECTION TO NETWORK\r\n\r\n"
#define HD_CONNECTION_TO_NODE               "CONNECTION TO NODE\r\n\r\n"
#define HD_GET_BLOCKS                       "GET BLOCKS\r\n\r\n"
#define HD_ACTUAL_HEIGHT                    "ACTUAL HEIGHT\r\n\r\n"
#define HD_SEND_BLOCK                       "SEND BLOCK\r\n\r\n"
#define HD_GET_PENDING_TRANSACTION_LIST     "GET PENDING TRANSACTION LIST\r\n\r\n"
#define HD_SEND_PENDING_TRANSACTION_LIST    "SEND PENDING TRANSACTION LIST\r\n\r\n"
#define HD_REJECT_DEMAND                    "REJECT DEMAND\r\n\r\n"
#define HD_GET_PENDING_TRANSACTION          "GET PENDING TRANSACTION\r\n\r\n"
#define HD_SEND_PENDING_TRANSACTION         "SEND PENDING TRANSACTION\r\n\r\n"
#define HD_SEND_EPOCH_BLOCK                 "SEND EPOCH BLOCK\r\n\r\n"
#define HD_SEND_VOTE                        "SEND VOTE\r\n\r\n"

// DEMAND
#define DD_GET_HEIGHT                  1
#define DD_GET_BLOCKS                  2
#define DD_SEND_TRANSACTION            3
#define DD_GET_TRANSACTION_LIST        4
#define DD_SEND_VOTE                   5
#define DD_SEND_EPOCH                  6

// MESSAGE INFO

#define SERVERMSG           printf("\033[0;31m[S]:\033[0m ");
#define CLIENTMSG           printf("\033[0;34m[C]:\033[0m ");
#define MANAGERMSG          printf("\033[0;32m[M]:\033[0m ");
#define WARNINGMSG(x)       printf("\033[0;35m[W]: %s\033[0m\n", x);

typedef struct __attribute__ ((__packed__)) get_blocks_t
{
    uint32_t version;
    char nb_demands;
    size_t blocks_height[50];
}get_blocks_t;

#endif