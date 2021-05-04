#ifndef CLIENT_TEST_C
#define CLIENT_TEST_C

#include <signal.h>
#include "tests_macros.h"

#include "network/network.h"
#include "network/server.h"
#include "network/client.h"
#include "network/send_data.h"
#include "network/get_data.h"

extern client_connection *client_connections;

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
        infos.serv_type = NODESERVER;
        char connection_type = 42;

        client_connections = malloc(sizeof(client_connection) * MAX_CONNECTION);
        if (listen_to(&infos, get_my_node(IM_CLIENT)->neighbours[0], &connection_type) != NULL)
        {
            TEST_PASSED("Connect to server");
        }
        else
        {
            TEST_FAILED("Connect to server", "listen_to(0) returned NULL");
        }
    }
}
#endif