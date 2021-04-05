#pragma once

#include <stdlib.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <time.h>

#define TRANSACTION_DATA_SIZE sizeof(size_t) * 3 + sizeof(time_t) + (512 * 2)
#define TRANSACTION_SIZE sizeof(size_t) + 2048 + TRANSACTION_DATA_SIZE
typedef struct TransactionData
{
    // All users area
    RSA *sender_public_key;          // The public key of the sender
    RSA *receiver_public_key;        // The public key of the receiver
    RSA *organisation_public_key;    // The public key of the organisation which will receive a part of the fees
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
    TransactionData *transaction_data; // Exclude the signature

    size_t signature_len;
    char *transaction_signature; // SHA384 signature
} Transaction;

/**
 * @brief Send 'amount' money to 'receiver_public_key'.
 * This will broadcast a transaction to the network
 * 
 * @param amount The amount to send
 * @param receiver_public_key The receiver public key
 * @return returns 0 if the broadcast succeeds, -1 otherwise
 */
int send_money(size_t amount, u_int64_t receiver_public_key);