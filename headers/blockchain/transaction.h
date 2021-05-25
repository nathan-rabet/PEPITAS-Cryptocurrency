#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string.h>
#include <stdlib.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include "blockchain/wallet.h"
#include "blockchain/blockchain_header.h"

#define TRANSACTION_DATA_SIZE sizeof(size_t) * 3 + sizeof(time_t) + (512 * 2)
#define TRANSACTION_SIZE sizeof(size_t) + 2048 + TRANSACTION_DATA_SIZE

#define T_TYPE_DEFAULT 0
#define T_TYPE_ADD_STAKE 1
#define T_TYPE_WITHDRAW_STAKE 2
#define T_TYPE_REWARD_STAKE 3
#define T_TYPE_PUNISH_STAKE 4

#ifndef TRANS_T
#define TRANS_T
typedef struct TransactionData
{
    char magic;
    char type;
    // All users area
    RSA *sender_public_key;          // The public key of the sender
    RSA *receiver_public_key;        // The public key of the receiver
    size_t amount;                   // The amount spent by the sender
    size_t sender_remaining_money;   // The money the sender have AFTER the transaction
    size_t receiver_remaining_money; // The money the receiver have AFTER the transaction
    time_t transaction_timestamp;    // The time when the transaction was crafted

    // Organisations: must indicates what you bought
    // Normal node: free 1024 bytes data
    char cause[512];
    char asset[512];
} TransactionData;

typedef struct Transaction
{
    TransactionData transaction_data; // Exclude the signature

    char transaction_signature[256]; // SHA384 signature
} Transaction;

#endif

/**
 * @brief Send 'amount' money to 'receiver_public_key'.
 * This will broadcast a transaction to the network
 * 
 * @param amount The amount to send
 * @param receiver_public_key The receiver public key
 * @return returns 0 if the broadcast succeeds, -1 otherwise
 */
int send_money(size_t amount, u_int64_t receiver_public_key);

void write_transactiondata(TransactionData *transaction, int fd);
void write_transaction(Transaction *transaction, int fd);
void get_transaction_data(Transaction *trans, char **buff, size_t *index);
void convert_data_to_transactiondata(TransactionData *transactiondata, int fd);
void load_transaction(Transaction *transaction, int fd);
Transaction* load_pending_transaction(time_t timestamp);
void add_pending_transaction(Transaction *transaction);
Transaction create_new_transaction(infos_st *infos, char type, RSA* receiver_public_key, size_t amount, char cause[512], char asset[512]);


/**
 * @brief Delete block transactions in pdt if the block is valid
 * 
 * @param transactions block.blockdata.transactions
 * @param nb_transactions number of transactions
 */
void flush_pending_transactions(Transaction **transactions, size_t nb_transactions);

#endif