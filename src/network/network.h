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
#include "../misc/safe.h"
#include "client.h"

#define STATIC_PORT "8080"
#define REQUEST_HEADER_SIZE 256

static const Neighbour HARD_CODED_IPS[] = {
    {AF_INET, "127.0.0.1"}};

// HEADERS
#define HD_GET_CLIENT_LIST "GET CLIENT LIST\r\n\r\n"
#define HD_REC_CLIENT_LIST "REC CLIENT LIST\r\n\r\n"
#define HD_GET_BLOCKCHAIN "GET BLOCKCHAIN\r\n\r\n"
#define HD_REC_BLOCKCHAIN "REC BLOCKCHAIN\r\n\r\n"
#endif