#include "blockchain/block.h"
#include "client.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"
#include "misc/safe.h"
#include "blockchain/transaction.h"
#include <openssl/rsa.h>
#include "ui/ui.h"
#include "blockchain/blockchain_header.h"

extern connection *client_connections;
static pthread_t server_t;
extern infos_st *ac_infos;

int main(int argc, char **argv)
{
    
    struct stat st = {0};

    if (stat("data", &st) == -1)
    {
        mkdir("data", 0700);
    }
    if (stat("data/pdt", &st) == -1)
    {
        mkdir("data/pdt", 0700);
    }
    if (stat("data/epoch", &st) == -1)
    {
        mkdir("data/epoch", 0700);
    }
    create_account();
    gtk_init(NULL, NULL);
    MANAGERMSG 
    printf("Starting UI\n");
    
    infos_st *infos = malloc(sizeof(infos_st));
    infos->actual_height = 0;
    infos->pdt = 0;
    infos->as_epoch = 2;
    infos->is_validator = 0;
    infos->validator_id = -1;
    infos->serv_type = NODESERVER;
    ac_infos = infos;
    
    pthread_t ui_th;
    pthread_create(&ui_th, NULL, setup, infos);

    while (infos->as_epoch == 2)
    {
        sleep(1);
    }



    client_connections = calloc(MAX_CONNECTION, sizeof(connection));
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        sem_init(&client_connections[i].lock, 0, 0);
    }
    MANAGERMSG
    printf("Starting Manager\n");
    MANAGERMSG
    printf("Try to load last client list\n");
    load_neighbours(IM_CLIENT);

    if (argc == 1 && number_neighbours(IM_CLIENT) == 0)
    {
        MANAGERMSG
        printf("No last node for the network :(\n");
        MANAGERMSG
        printf("Search on doors...\n");
        join_network_door(infos);
    }
    if (argc == 2 && number_neighbours(IM_CLIENT) == 0)
    {
        set_neighbour(IM_CLIENT, argv[1], AF_INET);
    }

    clear_transactions();
    clear_epochs();
    init_validators_state();
    // Try Load Old blockchain
    gen_blockchain_header(infos);
    update_sync(infos->actual_height+1, infos->actual_height+1);
    // Open server
    infos->serv_type = NODESERVER;
    pthread_create(&server_t, NULL, init_server, infos);
    

    // TEST LEN LIST
    if (number_neighbours(IM_CLIENT) == 0)
    {
        // FIRST NODE
        MANAGERMSG
        printf("I'am the first node on the network\n");
    }
    else
    {
        // SYNC TO OTHERS
        MANAGERMSG
        printf("Connection to others...\n");
        connection_to_others(infos);

        // SYNC BLOCKCHAIN
        MANAGERMSG
        printf("Update blockchain height...\n");
        size_t index_client = update_blockchain_height(infos);
        MANAGERMSG
        printf("Max client blockchain height found is: %lu\n", client_connections[index_client].actual_client_height);
        MANAGERMSG
        printf("Update blockchain...\n");
        update_blockchain(infos, index_client);
        MANAGERMSG
        printf("Blockchain Synchronized!\n New height: %lu\n", infos->actual_height+1);
        update_sync(infos->actual_height+1, infos->actual_height+1);

        // SYNC PDT
        MANAGERMSG
        printf("Update pending transactions list...\n");
        update_pending_transactions_list();
        MANAGERMSG
        printf("Pending transactions list synchronized!\n");

    }
    infos->validator_id = i_am_commitee_member();
    if (infos->validator_id < 0) {

        MANAGERMSG
        printf("You are not in the next comitte.\n");
    }
    else
    {
        MANAGERMSG
        printf("You are in the next comitte.\n");
    }

    pthread_exit(NULL);
}
