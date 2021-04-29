#include "network/client.h"
#include "network/network.h"
#include <arpa/inet.h>

const Neighbour HARD_CODED_ADDR[] =
{
    {AF_INET, "34.72.117.116"}, // GCP SERVER
    {AF_INET, "127.0.0.1"}  // LOCAL HOST
};