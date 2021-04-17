#include <signal.h>
#include <stdlib.h>

#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"

extern int connection_fd;
extern int nb_connection;
extern client_connection *client_connections;
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

void connection_to_others(){
    Node *node = get_my_node(IM_CLIENT);
    for (size_t i = 0; i < MAX_NEIGHBOURS && nb_connection < MAX_CONNECTION; i++)
    {
        if (node->neighbours[i].hostname != NULL)
        {
            listen_to(node->neighbours[i]);
        }
    }
    printf("Connected to %i clients! \n", nb_connection);
}

void askfornewblockchain()
{
    Node *node = get_my_node(IM_CLIENT);
    for (size_t i = 0; i < MAX_NEIGHBOURS; i++)
    {
        if (node->neighbours[i].hostname != NULL)
        {
        }
    }
    
}

int main()
{

    client_connections = calloc(MAX_SERVER, sizeof(client_connection));
    connection_fd = 0;
    nb_connection = 0;
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
        printf("Connection to others...\n");
        connection_to_others();
        printf("Update blockchain...\n");

    }
    
    while (1);
}