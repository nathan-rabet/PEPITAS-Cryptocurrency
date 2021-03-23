#include <stdlib.h>
#include <openssl/dsa.h>
typedef struct Transaction
{
    // All users area
    RSA *sender_public_key; // The public key of the sender
    RSA *receiver_public_key; // The public key of the receiver
    RSA *organisation_public_key; // The public key of the organisation which will receive a part of the fees
    size_t amount; // The amount spent by the sender
    uint64_t transaction_timestamp; // The time when the transaction was crafted

    // Organisations: must indicates what you bought
    // Normal node: free 1024 bytes data
    char cause[512];
    char asset[512];
} Transaction;

typedef struct PendingTransactionsLedger
{

} PendingTransactionsLedger;

/**
 * @brief Send 'amount' money to 'receiver_public_key'.
 * This will broadcast a transaction to the network
 * 
 * @param amount The amount to send
 * @param receiver_public_key The receiver public key
 * @return return 0 if the broadcast succeed, -1 if not
 */
int send_money(size_t amount, u_int64_t receiver_public_key);
