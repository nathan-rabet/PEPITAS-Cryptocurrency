#include "client.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"
#include "misc/safe.h"
#include "blockchain/blockchain_header.h"
#include "blockchain/transaction.h"
#include <openssl/rsa.h>
#include "ui/ui.h"

extern client_connection *client_connections;
static pthread_t server_t;
infos_st *ac_infos;

infos_st* get_infos(){
    return ac_infos;
}

void Validate(){
    if (ac_infos->is_validator == 0)
        return;
    Block *epoch = create_epoch_block();
    // SEND REQUEST DD_SEND_EPOCH
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd != 0)
        {
            while (client_connections[i].demand != 0)
                ;
            client_connections[i].demand = DD_SEND_EPOCH;
            client_connections[i].Payload = (void *)epoch;
            sem_post(&client_connections[i].lock);
        }
    }

    // WAIT
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd != 0)
        {
            while (client_connections[i].demand != 0)
                ;
            free(client_connections[i].Payload);
        }
    }

    MANAGERMSG
    printf("Create new epoch!\n");
}

void new_transaction(char type, char *rc_pk, size_t amount, char cause[512], char asset[512]){
    BIO *pubkey2 = BIO_new(BIO_s_mem());
    BIO_write(pubkey2, rc_pk, strlen(rc_pk));
    RSA* key = PEM_read_bio_RSAPublicKey(pubkey2, NULL, 0, NULL);
    BIO_free(pubkey2);
    if (!key)
    {
        MANAGERMSG
        printf("Can't create a the transaction with a non valid key!\n");
        return;
    }

    // TEST MONEY
    Wallet *wallet = get_my_wallet();
    if (type != T_TYPE_WITHDRAW_STAKE && amount > wallet->amount) {
        MANAGERMSG
        printf("Can't create a the transaction not enough money! (%lu tr / %lu bal)\n", amount, wallet->amount);
        return;
    }
    if (type == T_TYPE_WITHDRAW_STAKE && amount > wallet->stake_amount) {
        MANAGERMSG
        printf("Can't create a the transaction not enough money in the stake! (%lu tr / %lu bal)\n", amount, wallet->amount);
        return;
    }
    
    Transaction trans = create_new_transaction(ac_infos, type, key, amount, cause, asset);
    add_pending_transaction(&trans);

    // SEND PENDING TRANSACTION
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd != 0) {
            while (client_connections[i].demand != 0);
            client_connections[i].demand = DD_SEND_TRANSACTION;
            client_connections[i].Payload = malloc(sizeof(time_t));
            *(time_t *)client_connections[i].Payload = trans.transaction_data.transaction_timestamp;
            sem_post(&client_connections[i].lock);
        }
    }

    // WAIT
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd != 0) {
            while (client_connections[i].demand != 0);
            free(client_connections[i].Payload);
        }
    }

    // VALIDATOR TO REMOVE
    Validate();
}


void join_network_door(infos_st *infos){
    client_connection *connection_fd;
    for (size_t i = 0; i < NB_HARD_CODED_ADDR; i++)
    {
        connection_fd = listen_to(infos ,HARD_CODED_ADDR[i], HD_CONNECTION_TO_NETWORK);
        if (connection_fd != NULL)
            break;
    }
    if (connection_fd == NULL)
        err(EXIT_FAILURE, "Aie aie aie pas de réseau mon reuf :(\nHave a great day\n");

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
            if (listen_to(infos, node->neighbours[i], HD_CONNECTION_TO_NODE) == NULL)
                printf("Fail de connection to neighbour\n");
        }
    }
    MANAGERMSG
    printf("Connected to %i clients! \n", nb_connection);
    char tmp[5];
    snprintf(tmp, 5, "%i", nb_connection);
    change_label_text(connections_label, tmp);
}

size_t update_blockchain_height(infos_st *infos)
{
    size_t max_h = 0;
    size_t max_h_i = 0;
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd != 0) {
            get_blocks_t * Payload = calloc(1, sizeof(get_blocks_t));
            Payload->version = P_VERSION;
            Payload->nb_demands = 1;
            Payload->blocks_height[0] = 0;
            client_connections[i].demand = DD_GET_HEIGHT;
            client_connections[i].Payloadsize = sizeof(get_blocks_t);
            client_connections[i].Payload = Payload;
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
    update_sync(infos->actual_height, max_h_i);
    return max_h_i;
}

void update_blockchain(infos_st *infos, size_t index_client){
    size_t demand_height = 0;
    while (client_connections[index_client].actual_client_height > infos->actual_height + demand_height) {
        client_connections[index_client].demand = DD_GET_BLOCKS;

        char nb_dd = (client_connections[index_client].actual_client_height - infos->actual_height) % 50;
        MANAGERMSG
        printf("Demande de %i Blocks\n", nb_dd);

        client_connections[index_client].Payloadsize = sizeof(get_blocks_t);
        get_blocks_t * Payload = calloc(1, sizeof(get_blocks_t));
        client_connections[index_client].Payload = Payload;
        Payload->version = P_VERSION;
        Payload->nb_demands = nb_dd;
        for (char i = 1; i <= nb_dd; i++)
        {
            Payload->blocks_height[i-1] = infos->actual_height + i + demand_height;
        }
        

        demand_height += nb_dd;
        sem_post(&client_connections[index_client].lock);
        //WAIT
        while (client_connections[index_client].demand != 0);
    }

}

void update_pending_transactions_list(infos_st *infos){
    // CLEAR DIR
    infos->pdt = 0;
    char temp[100];
    DIR *d;
    struct dirent *dir;
    d = opendir("./pdt");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG)
            {
                sscanf(temp, "./pdt%hhu", dir->d_name);
                remove(temp);
            }
        }
        closedir(d);
    }
    

    // SYNC
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd != 0) {
            client_connections[i].demand = DD_GET_TRANSACTION_LIST;
            sem_post(&client_connections[i].lock);
        }
    }
    
    //WAIT
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd != 0) {
            while (client_connections[i].demand != 0);
        }
    }
    char tmp[10];
    snprintf(tmp, 10, "%li", infos->pdt);
    change_label_text(mempool_label, tmp);
}

int main()
{
    create_account();
    gtk_init(NULL, NULL);
    MANAGERMSG
    printf("Starting UI\n");
    
    infos_st *infos = malloc(sizeof(infos_st));
    infos->actual_height = 0;
    infos->pdt = 0;
    infos->is_sychronize = 2;
    infos->is_validator = 0;
    infos->validator_id = -1;
    infos->serv_type = NODESERVER;
    ac_infos = infos;
    
    pthread_t ui_th;
    pthread_create(&ui_th, NULL, setup, &infos);

    while (infos->is_sychronize == 2)
    {
        sleep(1);
    }



    client_connections = calloc(MAX_CONNECTION, sizeof(client_connection));
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        sem_init(&client_connections[i].lock, 0, 0);
    }
    MANAGERMSG
    printf("Starting Manager\n");
    MANAGERMSG
    printf("Try to load last client list\n");
    load_neighbours(IM_CLIENT);

    if (number_neighbours(IM_CLIENT) == 0)
    {
        MANAGERMSG
        printf("No last node for the network :(\n");
        MANAGERMSG
        printf("Search on doors...\n");
        join_network_door(infos);
    }

    // Try Load Old blockchain
    gen_blockchain_header(infos);
    update_sync(infos->actual_height, infos->actual_height);
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
        printf("Blockchain syncronized with: %lu\n", infos->actual_height);
        change_label_text(synchro_label, "Syncronized");
        change_label_text(block_amount_label, "");

        // SYNC PDT
        MANAGERMSG
        printf("Update pending transactions list...\n");
        update_pending_transactions_list(infos);
        MANAGERMSG
        printf("Pending transactions list syncronized!\n");

    }

    pthread_exit(NULL);
}