#include "../src/network/network.h"

int init_server_test()
{
    printf("Testing init_server()\n'");
    return init_server(STATIC_DNS);
}