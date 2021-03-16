#include <stdlib.h>
#include "transaction.h"

#define NB_TRANSACTIONS_PER_BLOCK 100

typedef struct Block
{
    // Clasical implementation
    struct Block *previous_block;
    Transaction transactions[NB_TRANSACTIONS_PER_BLOCK];
    struct Block *next_block;

    // Validator area
    u_int64_t validator_public_key;
    u_int64_t validation_timestamp;

    // Open data (you can put whatever you want)
    char opendata[1024];
} Block;