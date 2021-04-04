#include <signal.h>
#include "tests_macros.h"

#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"

void network_test()
{
    // Init client lists
    if (set_neighbour(NULL, 0) == 0)
    {
        TEST_PASSED("Init hard coded addresses");
    }
    else
    {
        TEST_WARNING("Init hard coded addresses", "All hard-coded servers are down");
    }

    if (listen_to(0) == 0)
    {
        TEST_PASSED("Connect to server");
    }
    else
    {
        TEST_WARNING("Connect to server", "listen_to(0) returned -1");
    }
}