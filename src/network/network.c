#include "network/client.h"
#include <arpa/inet.h>

const Neighbour HARD_CODED_ADDR[] =
{
    {AF_INET, "34.72.117.116", 0}, // GCP SERVER
    {AF_INET, "127.0.0.1", 0}  // LOCAL HOST
};