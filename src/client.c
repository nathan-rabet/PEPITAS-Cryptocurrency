#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"
#include "misc/safe.h"
#include "blockchain/blockchain_header.h"

extern client_connection *client_connections;
static pthread_t server_t;



void join_network_door(infos_st *infos){
    client_connection *connection_fd;
    for (size_t i = 0; i < NB_HARD_CODED_ADDR; i++)
    {
        connection_fd = listen_to(infos ,HARD_CODED_ADDR[i]);
        if (connection_fd != NULL)
            break;
    }
    if (connection_fd == NULL)
        err(EXIT_FAILURE, "Aie aie aie pas de réseau mon reuf :(\nHave a great day\n");

    //SEND ACCEPT
    safe_write(connection_fd->clientfd, HD_CONNECTION_TO_NETWORK, strlen(HD_CONNECTION_TO_NETWORK));

    read_header(connection_fd->clientfd, infos);
    print_neighbours(IM_CLIENT, 0);

    // Close connection to door server
    close(connection_fd->clientfd);
    pthread_cancel(connection_fd->thread);
    connection_fd->clientfd = 0;
}

void connection_to_others(infos_st *infos){
    Node *node = get_my_node(IM_CLIENT);
    int nb_connection = number_neighbours(IM_CLIENT);
    for (size_t i = 0; i < MAX_NEIGHBOURS && nb_connection < MAX_CONNECTION; i++)
    {
        if (node->neighbours[i].hostname != NULL)
        {
            if (listen_to(infos, node->neighbours[i]) == NULL)
                printf("Fail de connection to neighbourg\n");
        }
    }
    printf("Connected to %i clients! \n", nb_connection);
}

size_t update_blockchain_height(infos_st *infos)
{
    size_t max_h = 0;
    size_t max_h_i = 0;
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd != 0) {
            client_connections[i].demand = DD_GET_HEIGHT;
            client_connections[i].Playloadsize = sizeof(uint32_t) + sizeof(char) + sizeof(size_t);
            client_connections[i].Payload = malloc(client_connections[i].Playloadsize);
            *(uint32_t *)client_connections[i].Payload = P_VERSION;
            *(char *)(client_connections[i].Payload + sizeof(uint32_t)) = 1;
            *(size_t *)(client_connections[i].Payload + sizeof(size_t) + sizeof(char)) = 0;
            sem_post(&client_connections[i].lock);

            // BREAK IF SUCCESS
        }
    }
    
    //WAIT
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd != 0) {
            while (client_connections[i].demand != 0);
            if (max_h < client_connections[i].actual_client_height){
                max_h = client_connections[i].actual_client_height;
                max_h_i = i;
            }
            free(client_connections[i].Payload);
        }
    }

    return max_h_i;
}

void update_blockchain(infos_st *infos, size_t index_client){

    while (client_connections[index_client].actual_client_height < infos->actual_height) {
        client_connections[index_client].demand = DD_GET_BLOCKS;

        char nb_dd = (infos->actual_height - client_connections[index_client].actual_client_height) % 50;
        printf("Demande de %i Blocks\n", nb_dd);

        client_connections[index_client].Playloadsize = sizeof(uint32_t) + sizeof(char) + (sizeof(size_t) * nb_dd);
        client_connections[index_client].Payload = malloc(client_connections[index_client].Playloadsize);
        *(uint32_t *)client_connections[index_client].Payload = P_VERSION;
        *(char *)(client_connections[index_client].Payload + sizeof(uint32_t)) = nb_dd;
        for (size_t i = 1; i <= nb_dd; i++)
        {
            *(size_t *)(client_connections[index_client].Payload + sizeof(uint32_t) + sizeof(char) + (sizeof(size_t) * i)) = client_connections[index_client].actual_client_height + i;
        }
        

        sem_post(&client_connections[index_client].lock);
        //WAIT
        while (client_connections[index_client].demand != 0);
    }

}

int main()
{
    infos_st *infos = malloc(sizeof(infos_st));
    infos->actual_height = 0;
    infos->is_sychronize = 0;
    infos->serv_type = NODESERVER;

    client_connections = calloc(MAX_CONNECTION, sizeof(client_connection));
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        sem_init(&client_connections[i].lock, 0, 0);
    }

    printf("Starting client...\n");

    printf("Try to load last client list\n");
    load_neighbours(IM_CLIENT);

    if (number_neighbours(IM_CLIENT) == 0)
    {
        printf("No last node for the network :(\nSearch on doors...\n");
        join_network_door(infos);
    }

    // Try Load Old blockchain
    gen_blockchain_header(infos);

    // Open server
    pthread_create(&server_t, NULL, init_server, &infos);

    // TEST LEN LIST
    if (number_neighbours(IM_CLIENT) == 0)
    {
        printf("I'am the first node on the network\n");

    }
    else
    {
        printf("Connection to others...\n");
        connection_to_others(infos);
        printf("Update blockchain height...\n");
        size_t index_client = update_blockchain_height(infos);
        printf("Max client blockchain height found is: %lu\n", client_connections[index_client].actual_client_height);
        printf("Update blockchain...\n");
        update_blockchain(infos, index_client);
        printf("Blockchain syncronized with: %lu\n", infos->actual_height);
    }
    
    while (1);
}