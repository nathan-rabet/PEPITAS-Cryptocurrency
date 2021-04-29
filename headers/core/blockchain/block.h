#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <openssl/sha.h>
#include "transaction.h"
#include "core/validation/validations.h"

#define CURRENT_CHUNK 0

#define BLOCK_DATA_SIZE (SHA384_DIGEST_LENGTH * 2 + 1) + sizeof(size_t) + sizeof(uint16_t) + sizeof(time_t)
#define BLOCK_SIZE 2048 + sizeof(size_t) + BLOCK_DATA_SIZE + SHA384_DIGEST_LENGTH * 2 + 1

#define MAX_TRANSACTIONS_PER_BLOCK 16384
#define NB_BLOCK_PER_CHUNK 10000

// Standard implementation
typedef struct BlockData
{
    char magic;                                             // Describe the block type
    int epoch_id;                                           // EPOCH MAN
    char is_prev_block_valid;                               // EPOCH MAN VOTE ON LAST BLOCK
    char previous_block_hash[SHA384_DIGEST_LENGTH * 2 + 1]; // Previous block SHA384 hash
    size_t height;                                          // The height of the block inside the blockchain

    uint16_t nb_transactions;   // The number of validated transactions inside the current block
    Transaction **transactions; // Transactions vector

    //* Validator area
    RSA *validator_public_key[MAX_VALIDATORS_PER_BLOCK];            // The public key of the validators
    char prev_validators_votes[MAX_VALIDATORS_PER_BLOCK / 8]; // BITMAP
    time_t block_timestamp;                                         // The block creation timestamp
} BlockData;

typedef struct Block
{
    uint16_t chunk_id;
    BlockData block_data; // The block distributed data, excluding the block signature

    char block_signature[256]; // SHA384 signature EPOCHMAN

    //Validator Vote
    char validators_votes[MAX_VALIDATORS_PER_BLOCK / 8]; // BITMAP
    char vote_signature[MAX_VALIDATORS_PER_BLOCK - 1][256];
} Block;

typedef struct ChunkBlockchain
{
    size_t chunk_nb; // The split offset
    Block **chunk;   // The splited blocks
} ChunkBlockchain;

/**
 * @brief Loads a blockchain object with a padding of 'nb_chunk'
 * 
 * @param nb_chunk The chunk nb, 
 * if 0 : return the current blockchain object without modification
 * @return ChunkBlockchain*, NULL if the ChunkBlockchain is empty after switching
 */
ChunkBlockchain *get_blockchain(size_t nb_chunk);
/**
 * @brief Writes a block struct in a file
 * 
 * @param block The block to write
 */
void write_block_file(Block block);

Block *get_block(size_t block_height);

/**
 * @brief Free a block struct
 * 
 * @param block The block to free
 */
void free_block(Block *block);

/**
 * @brief For a block of height `h`, returns the block of height `h+1`
 * 
 * @param block The base block
 * @return The next Block* 
 */
Block *get_next_block(Block *block);

/**
 * @brief For a block of height `h`, return the block of height `h-1`
 * 
 * @param block The base block
 * @return The next Block* 
 */
Block *get_prev_block(Block *block);
#endif