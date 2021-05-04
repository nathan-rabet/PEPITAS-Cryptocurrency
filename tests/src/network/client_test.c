#ifndef CLIENT_TEST_C
#define CLIENT_TEST_C

#include <signal.h>
#include "tests_macros.h"

#include "network/network.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"

void network_test()
{
    // Init client lists
    char areDown = 0;
    if (set_neighbour(IM_CLIENT, HARD_CODED_ADDR->hostname, HARD_CODED_ADDR->family) == 0)
    {
        TEST_PASSED("Init hard coded addresses");
    }
    else
    {
        areDown = 1;
        TEST_WARNING("Init hard coded addresses", "All hard-coded servers are down");
    }

    if (!areDown)
    {
        infos_st infos;
        char connection_type = 42;
        if (listen_to(&infos,get_my_node(IM_CLIENT)->neighbours[0],&connection_type)->clientfd != 0)
        {
            TEST_PASSED("Connect to server");
        }
        else
        {
            TEST_FAILED("Connect to server", "listen_to(0) returned -1");
        }
    }
}
#endif