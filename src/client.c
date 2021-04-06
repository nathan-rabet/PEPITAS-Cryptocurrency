#include <signal.h>
#include <stdlib.h>

#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"

int main()
{
    if (set_neighbour(NULL, 0) != 0)
    {
        errx(EXIT_FAILURE,"All servers are down");
    }
    fetch_client_list(0);
    return 0;
}