#include <signal.h>
#include <stdlib.h>

#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"

extern int connection_fd;
static pthread_t server_t;


void join_network_door(){
    for (size_t i = 0; i < NB_HARD_CODED_ADDR; i++)
    {
        if (listen_to(HARD_CODED_ADDR[i]) == 0)
            break;
    }
    if (connection_fd == 0)
        err(EXIT_FAILURE, "Aie aie aie pas de réseau mon reuf :(\nHave a great day\n");

    //SEND ACCEPT
    safe_write(connection_fd, HD_CONNECTION_TO_NETWORK, strlen(HD_CONNECTION_TO_NETWORK));

    read_header(connection_fd);
    print_neighbours(IM_CLIENT, 0);

    // Close connection to door server
    close(connection_fd);
    connection_fd = 0;

}

int main()
{
    connection_fd = 0;
    printf("Starting client...\n");

    printf("Try to load last client list\n");
    load_neighbours(IM_CLIENT);

    if (number_neighbours(IM_CLIENT) == 0)
    {
        printf("No last node for the network :(\nSearch on doors...\n");
        join_network_door();
    }
    // Try Load Old blockchain

    // Open server
    char type = NODESERVER;
    pthread_create(&server_t, NULL, init_server, &type);

    // TEST LEN LIST
    if (number_neighbours(IM_CLIENT) == 0)
    {
        printf("I'am the first node on the network\n");

    }
    else
    {
        printf("Update blockchain...\n");

    }
    
    while (1);
}