#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <openssl/sha.h>
#include "transaction.h"

#define MAX_TRANSACTIONS_PER_BLOCK 65535
#define NB_BLOCK_PER_CHUNK 10000

// Standard implementation
typedef struct BlockData
{
    char previous_block_hash[SHA384_DIGEST_LENGTH * 2 + 1]; // Previous block SHA384 hash
    size_t height;                                          // The height of the block inside the blockchain

    uint16_t nb_transactions;  // The number of validated transactions inside the current block
    Transaction *transactions; // Transactions vector

    //* Validator area
    RSA *validator_public_key; // The public key of the validator
    time_t block_timestamp;    // The block creation timestamp
} BlockData;

typedef struct Block
{
    struct Block *previous_block; // Previous block pointer
    struct Block *next_block;     // Next block pointer

    BlockData block_data; // The block distributed data, excluding the block signature

    char next_block_hash[SHA384_DIGEST_LENGTH * 2 + 1]; // Next block SHA384 hash

    size_t signature_len;  // The length of the signature
    char *block_signature; // SHA384 signature

} Block;

typedef struct ChunkBlockchain
{
    size_t chunk_nb;                 // The split offset
    Block chunk[NB_BLOCK_PER_CHUNK]; // The splited blocks
} ChunkBlockchain;

/**
 * @brief Load a blockchain object with a padding of 'nb_chunk'
 * 
 * @param nb_chunk The chunk nb, 
 * if 0 : return the current blockchain object without modification
 * @return ChunkBlockchain 
 */
ChunkBlockchain get_blockchain(size_t nb_chunk);
#endif