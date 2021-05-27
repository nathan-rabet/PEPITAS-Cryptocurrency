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
infos_st *ac_infos;

infos_st* get_infos(){
    return ac_infos;
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
        printf("Can't create a the transaction not enough money!\n");
        return;
    }
    if (type == T_TYPE_WITHDRAW_STAKE && amount > wallet->stake_amount) {
        MANAGERMSG
        printf("Can't create a the transaction not enough money in the stake!\n");
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
}

int main()
{
    Wallet *wallet = get_my_wallet();
    create_account();
    Block genesis_block;
    genesis_block.block_data.magic = 0;
    genesis_block.block_data.height = 0;
    genesis_block.block_data.epoch_id = 0;
    genesis_block.block_data.validators_public_keys[0] = wallet->pub_key;
    genesis_block.block_data.nb_validators = 1;
    genesis_block.block_data.is_prev_block_valid = 1;

    // TRANSACTION
    genesis_block.block_data.transactions = malloc(2 * sizeof(Transaction));
    genesis_block.block_data.nb_transactions = 2;

    Transaction new_trans;
    TransactionData *data = &new_trans.transaction_data;
    data->magic = 1;
    data->type = T_TYPE_ADD_STAKE;
    data->sender_public_key = wallet->pub_key;
    data->receiver_public_key = NULL;
    data->amount = 4200000000;
    data->sender_remaining_money = 0;
    data->receiver_remaining_money = 4200000000;
    data->transaction_timestamp = time(NULL);
    memcpy(data->cause, "Let's go", 9);
    memcpy(data->asset, "First transaction!!!!!!!!!!!", 29);

    Transaction new_trans2;
    data = &new_trans2.transaction_data;
    data->magic = 1;
    data->type = T_TYPE_DEFAULT;
    data->sender_public_key = NULL;
    data->receiver_public_key = wallet->pub_key;
    data->amount = 100000000000;
    data->sender_remaining_money = 100000000000;
    data->receiver_remaining_money = 100000000000;
    data->transaction_timestamp = time(NULL);
    memcpy(data->cause, "RICH", 5);
    memcpy(data->asset, "Oui", 4);
    
    genesis_block.block_data.transactions[0] = &new_trans;
    genesis_block.block_data.transactions[1] = &new_trans2;
    sign_transaction_with_key(genesis_block.block_data.transactions[0], wallet->priv_key);
    sign_transaction_with_key(genesis_block.block_data.transactions[1], wallet->priv_key);

    genesis_block.block_data.block_timestamp = time(NULL);
    genesis_block.validators_votes[0] = 1;
    sign_block_with_key(&genesis_block, wallet->priv_key);
    char data2[900];
    BIO *my_pub = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(my_pub, genesis_block.block_data.validators_public_keys[0]);
    size_t index = BIO_pending(my_pub);
    memcpy(data2, &index, sizeof(size_t));
    BIO_read(my_pub, data + sizeof(size_t), index);
    index += sizeof(size_t);
    memcpy(data2 + index, &genesis_block.block_data.height, sizeof(size_t));
    index += sizeof(size_t);
    memcpy(data2 + index, &genesis_block.block_data.epoch_id, sizeof(int));
    index += sizeof(int);
    data2[index] = 1;
    index += sizeof(char);
    sign_message_with_key(genesis_block.vote_signature[0], index, wallet->priv_key, genesis_block.vote_signature[0]);

    write_block_file(genesis_block);
    return 0;
}