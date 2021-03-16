#include <stdlib.h>
typedef struct Transaction
{
    // All users area
    u_int64_t sender_public_key;
    u_int64_t receiver_public_key;
    u_int64_t organisation_public_key;
    size_t amount;

    // Organisations area
    char cause[256];
    char bought_asset[256];

    // Opendata (you can put whatever you want)
    char opendata[1024];
} Transaction;

/**
 * @brief Send 'amount' money to 'receiver_public_key'.
 * This will broadcast a transaction to the network
 * 
 * @param amount The amount to send
 * @param receiver_public_key The receiver public key
 * @return return 0 if the broadcast succeed, -1 if not
 */
int send_money(size_t amount, u_int64_t receiver_public_key);

