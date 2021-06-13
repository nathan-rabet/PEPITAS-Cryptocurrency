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
infos_st *ac_infos;

infos_st* get_infos(){
    return ac_infos;
}

void move_file(char *src, char* dest) {
    if (access(src, F_OK)) {

        CLIENTMSG
        printf("The file %s does not exist.\n", src);
        return;
    }
    int src_fd = open(src, O_RDONLY, 0644);
    if (src_fd == -1) {
        CLIENTMSG
        printf("Impossible to open %s\n", src);
        return;
    }
    int dest_fd = open(dest, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (src_fd == -1) {
        CLIENTMSG
        printf("Impossible to create %s\n", dest);
        return;
    }
    ssize_t r = 0;
    char buff[2000];
    while ((r = read(src_fd, buff, 2000)))
    {
        if (r == -1)
        {
            CLIENTMSG
            printf("Uneable to read %s\n", dest);
            return;
        }
        safe_write(dest_fd, buff, r);
    }
    close(src_fd);
    close(dest_fd);
}

void Validate(){
    if (ac_infos->is_validator == 0 || ac_infos->as_epoch == 1)
        return;
    Block *epoch = create_epoch_block();
    if (plebe_verify_block(epoch)) {

        CLIENTMSG
        printf("The block %lu is not valid.\n", epoch->block_data.height);
        return;
    }

    struct stat st = {0};
    char dir[300];

    if (stat("data/epoch", &st) == -1)
    {
        mkdir("data/epoch", 0700);
    }
    snprintf(dir, 300, "data/epoch/epoch%luid%d", epoch->block_data.height, epoch->block_data.epoch_id);

    if (!access(dir, F_OK)) {

        CLIENTMSG
        printf("The epoch %lu id %d is already existing.\n", epoch->block_data.height, epoch->block_data.epoch_id);
        return;

    }

    int fd = open(dir, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        err(errno, "Impossible to write epoch");
    write_block(*epoch, fd);
    
    ac_infos->as_epoch = 1;

    // SEND REQUEST DD_SEND_EPOCH
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd)
        {
            while (client_connections[i].demand)
                ;
            client_connections[i].demand = DD_SEND_EPOCH;
            client_connections[i].Payload = (void *)epoch;
            sem_post(&client_connections[i].lock);
        }
    }

    // WAIT
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd)
        {
            while (client_connections[i].demand)
                ;
        }
    }

    MANAGERMSG
    printf("Create new epoch!\n");

    // IF SOLO LAUCH TIMER TODO

    // if (epoch->block_data.nb_validators <= 1)
    // {
    //     epoch_validation_process(fd, epoch->block_data.height, epoch->block_data.epoch_id);
    //     // IS NEXT BLOCK
    //     if (ac_infos->actual_height + 1 == epoch->block_data.height)
    //     {
    //         ac_infos->is_validator++;
    //     }
    //     // update_wallet_with_block(*epoch);
    //     // write_block_file(*epoch);
    //     // ac_infos->actual_height++;
    //     // MANAGERMSG
    //     // printf("Block %lu is added in the blockchain!\n", epoch->block_data.height);
    // }
    epoch_validation_process(fd, epoch->block_data.height, epoch->block_data.epoch_id);
    free_block(epoch);
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
    sign_transaction(&trans);
    add_pending_transaction(&trans);
    get_infos()->pdt++;

    // SEND PENDING TRANSACTION
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd) {
            while (client_connections[i].demand);
            client_connections[i].demand = DD_SEND_TRANSACTION;
            client_connections[i].Payload = malloc(sizeof(time_t));
            *(time_t *)client_connections[i].Payload = trans.transaction_data.transaction_timestamp;
            sem_post(&client_connections[i].lock);
        }
    }

    // WAIT
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd) {
            while (client_connections[i].demand);
            free(client_connections[i].Payload);
        }
    }

    // VALIDATOR TO REMOVE
    Validate();
}


void join_network_door(infos_st *infos){
    connection* connection = NULL;
    for (size_t i = 0; i < NB_HARD_CODED_ADDR; i++)
    {
        if ((connection = listen_to(infos ,HARD_CODED_ADDR[i], HD_CONNECTION_TO_NETWORK, NULL)) != NULL)
            break;
    }
    if (connection == NULL)
        err(EXIT_FAILURE, "Aie aie aie pas de réseau mon reuf :(\nHave a great day\n");

    read_header(connection->clientfd, infos);
    print_neighbours(IM_CLIENT, 0);

    // Close connection to door server
    close(connection->clientfd);
    free(connection);
}

void connection_to_others(infos_st *infos){
    Node *node = get_my_node(IM_CLIENT);
    int nb_connection = number_neighbours(IM_CLIENT);
    for (size_t i = 0; i < MAX_NEIGHBOURS && nb_connection < MAX_CONNECTION; i++)
    {
        if (node->neighbours[i].hostname != NULL)
        {
            if (listen_to(infos, node->neighbours[i], HD_CONNECTION_TO_NODE, client_connections) == NULL)
                printf("Connection to neighbour failed\n");     
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
        if (client_connections[i].clientfd) {
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
        if (client_connections[i].clientfd) {
            while (client_connections[i].demand);
            if (max_h < client_connections[i].actual_client_height){
                max_h = client_connections[i].actual_client_height;
                max_h_i = i;
            }
            free(client_connections[i].Payload);
        }
    }
    update_sync(infos->actual_height+1, max_h_i);
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
        while (client_connections[index_client].demand);
    }

}
void clear_transactions()
{
    // CLEAR DIR
    char temp[256];
    DIR *d;
    struct dirent *dir;
    d = opendir("data/pdt");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG)
            {
                time_t txid = atol(dir->d_name);
                snprintf(temp, 256, "data/pdt/%ld", txid);
                remove(temp);
            }
        }
        closedir(d);
    }
}

void clear_epochs()
{
    // CLEAR DIR
    char temp[300];
    DIR *d;
    struct dirent *dir;
    d = opendir("data/epoch");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG)
            {
                snprintf(temp, 300, "data/epoch/%s", dir->d_name);
                remove(temp);
            }
        }
        closedir(d);
    }
}

void update_pending_transactions_list(infos_st *infos){
    
    clear_transactions();
    // SYNC
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd) {
            client_connections[i].demand = DD_GET_TRANSACTION_LIST;
            sem_post(&client_connections[i].lock);
        }
    }
    
    //WAIT
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd) {
            while (client_connections[i].demand);
        }
    }
    char tmp[10];
    snprintf(tmp, 10, "%li", infos->pdt);
    change_label_text(mempool_label, tmp);
}