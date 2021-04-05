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
#include <pthread.h>
#include <arpa/inet.h>
#include "misc/safe.h"
#include "client.h"

#define NB_HARD_CODED_ADDR 2
extern const Neighbour HARD_CODED_ADDR[];

#define STATIC_PORT "4242"

// REQUESTS
#define HD_GET_CLIENT_LIST "GET CLIENT LIST\r\n\r\n"
#define HD_SEND_CLIENT_LIST "SEND CLIENT LIST\n"
#define HD_GET_BLOCKCHAIN "GET BLOCKCHAIN\r\n\r\n"
#define HD_SEND_BLOCKCHAIN "SEND BLOCKCHAIN\n"

#endif