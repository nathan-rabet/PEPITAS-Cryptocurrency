#ifndef NETWORK_H
#define NETWORK_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <err.h>
#include <string.h>

#define USER_LIST_SIZE 100
#define STATIC_DNS "localhost"
#define STATIC_PORT "29562"
#define BUF_SIZE 256


int ConnectionToNetwork(char* name, char* port);

#endif