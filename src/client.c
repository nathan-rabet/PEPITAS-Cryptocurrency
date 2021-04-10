#include <signal.h>
#include <stdlib.h>

#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"

extern int connection_fd;


void join_network_door(){
    for (size_t i = 0; i < NB_HARD_CODED_ADDR; i++)
    {
        if (listen_to(HARD_CODED_ADDR[i]) == 0)
            break;
    }
    if (connection_fd == 0)
        err(EXIT_FAILURE, "Aie aie aie pas de réseau mon reuf :(\nHave a great day\n");
    read_header(connection_fd);
    print_neighbours();
}

int main()
{
    connection_fd = 0;
    printf("Starting client...\n");
    load_neighbours();
    if (number_neighbours() == 0)
    {
        printf("No last node for the network :(\nSearch on doors...\n");
        join_network_door();
    }
    
    return 0;
}