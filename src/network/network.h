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

#define USER_LIST_SIZE 100
#define STATIC_DNS "localhost"
#define STATIC_PORT "29562"
#define BUF_SIZE 256

struct Clientdata
{
    int family; // Use AF_*
    char* name;
    char len_name;
    char* port;
    char len_port;
};


/*
Try to connect at the name address and port.
Return socket FD if successful -1 if not
*/
int ConnectionToNetwork(char *name, char *port);

int GetClientData(int sockfd, struct Clientdata *clientdata, size_t clientdatasize, size_t start);

int InitServer(char* name, char* port);

#endif