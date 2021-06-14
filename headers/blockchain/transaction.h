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
    char magic; // A magic number, never used, but could be :)
    char type;  // The type of transaction (to user, to stake, from stake, ...)
    // All users area
    RSA *sender_public_key;          // The public key of the sender
    RSA *receiver_public_key;        // The public key of the receiver
    size_t amount;                   // The amount spent by the sender
    size_t sender_remaining_money;   // The money the sender have AFTER the transaction
    size_t receiver_remaining_money; // The money the receiver have AFTER the transaction
    time_t transaction_timestamp;    // The time when the transaction was crafted

    // Organisations: must indicates what you bought
    // Normal node: free 1024 bytes data
    char cause[512]; // Never used
    char asset[512]; // Never used
} TransactionData;

typedef struct Transaction
{
    TransactionData transaction_data; // Transaction data, excluding the signature

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

/**
 * @brief Serialize a TransactionData* structure
 * 
 * @param transaction The TransactionData* structure to serialize
 * @param fd The output file FD
 */
void write_transactiondata(TransactionData *transaction, int fd);

/**
 * @brief Serialize a Transaction* structure
 * 
 * @param transaction The Transaction* structure to serialize
 * @param fd The output file FD
 */
void write_transaction(Transaction *transaction, int fd);

/**
 * @brief Get the transaction data object
 * 
 * @param trans The returned transaction
 * @param buff The buffer with the serialized data
 * @param index The buffer starting offset
 */
void get_transaction_data(Transaction *trans, char **buff, size_t *index);

/**
 * @brief Convert serialized TransactionData* to TransactionData*
 * 
 * @param transactiondata The returned TransactionData*
 * @param fd The serialized TransactionData FD
 */
void convert_data_to_transactiondata(TransactionData *transactiondata, int fd);

/**
 * @brief Load a serialized Transaction* structure
 * 
 * @param transaction The returned Transaction*
 * @param fd The serialized Transaction FD
 */
void load_transaction(Transaction *transaction, int fd);

/**
 * @brief Load a transaction in the pending transaction (pdt) directory
 * 
 * @param timestamp The timestamp of the transaction
 * @return Transaction* 
 */
Transaction *load_pending_transaction(time_t timestamp);

/**
 * @brief Add a transaction to the pending transaction (pdt) directory
 * 
 * @param transaction The transaction to add
 */
void add_pending_transaction(Transaction *transaction);

/**
 * @brief Create a new transaction
 * 
 * @param infos Shared information object
 * @param type The type of transaction
 * @param receiver_public_key The receiver pkey
 * @param amount The amount of PEPITAS
 * @param cause The cause (deprecated)
 * @param asset The asset (deprecated)
 * @return Transaction 
 */
Transaction create_new_transaction(infos_st *infos, char type, RSA *receiver_public_key, size_t amount, char cause[512], char asset[512]);

/**
 * @brief Delete block transactions in the pending transaction (pdt) directory if the block is valid
 * 
 * @param transactions block.blockdata.transactions
 * @param nb_transactions number of transactions
 */
void flush_pending_transactions(Transaction **transactions, size_t nb_transactions);

#endif