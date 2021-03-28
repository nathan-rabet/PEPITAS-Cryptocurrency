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
        TEST_FAILED("Init hard coded addresses", "set_neighbour() returned -1, get_my_node()->neighbours == %p, get_my_node()->neighbours[0].client_sockfd == %d", get_my_node()->neighbours, get_my_node()->neighbours[0].client_sockfd);
    }

    if (listen_to(0) == 0)
    {
        TEST_PASSED("Connect to server");
    }
    else
    {
        TEST_FAILED("Connect to server", "listen_to(0) returned -1");
    }
    read_header(get_my_node()->neighbours->client_sockfd);
}