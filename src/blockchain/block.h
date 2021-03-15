#include <stdlib.h>
#define NB_TRANSACTIONS_PER_BLOCK 100

typedef struct Block
{
    // Clasical implementation
    Block *previous_block;
    Transaction transactions[NB_TRANSACTIONS_PER_BLOCK];
    Block *next_block;

    // Validator area
    u_int64_t validator_public_key;
    u_int64_t validation_timestamp;
} Block;

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
    char opendata[2048];
} Transaction;
