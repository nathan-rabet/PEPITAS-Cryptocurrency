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

#define STATIC_DNS "127.0.0.1"
#define STATIC_PORT "8080"
#define REQUEST_HEADER_SIZE 256

// HEADERS
#define HD_GET_CLIENT_LIST "GET CLIENT LIST"
#define HD_REC_CLIENT_LIST "REC CLIENT LIST"
#define HD_GET_BLOCKCHAIN "GET BLOCKCHAIN"
#define HD_REC_BLOCKCHAIN "REC BLOCKCHAIN"