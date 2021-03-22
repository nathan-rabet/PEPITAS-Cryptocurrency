#include <stdlib.h>
#include "transaction.h"

#define MAX_TRANSACTIONS_PER_BLOCK 65535
#define NB_BLOCK_PER_CHUNK 10000

struct uint256_t
{
    uint64_t bits[4];
};
typedef struct Block
{
    //* C Structure pointers
    struct Block *previous_block; // Previous block pointer
    struct Block *next_block;     // Next block pointer

    //* Standard implementation
    ssize_t height; // The height of the block inside the blockchain

    uint16_t nb_transactions;  // The number of validated transactions inside the current block
    Transaction *transactions; // Transactions vector

    //* Validator area
    u_int64_t validator_public_key; // The public key of the validator
    u_int64_t block_timestamp;      // The block creation timestamp

} Block;

typedef struct ChunkBlockchain
{
    size_t chunk_nb;                 // The split offset
    Block chunk[NB_BLOCK_PER_CHUNK]; // The splited blocks
} ChunkBlockchain;