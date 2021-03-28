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

#define STATIC_PORT "8080"
#define REQUEST_HEADER_SIZE 256

#define NB_HARD_CODED_ADDR 1
static const Neighbour HARD_CODED_ADDR[] =
    {
        {AF_INET, "34.72.117.116", 0, 0},
        {AF_INET, "127.0.0.1", 0, 0}};

// REQUESTS
#define HD_GET_CLIENT_LIST "GET CLIENT LIST\r\n\r\n"
#define HD_SEND_CLIENT_LIST "SEND CLIENT LIST\r\n\r\n"
#define HD_GET_BLOCKCHAIN "GET BLOCKCHAIN\r\n\r\n"
#define HD_SEND_BLOCKCHAIN "SEND BLOCKCHAIN\r\n\r\n"
#define HD_PING "PING \r\n\r\n"
#endif