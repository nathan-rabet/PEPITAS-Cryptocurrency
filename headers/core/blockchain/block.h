#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <openssl/sha.h>
#include "transaction.h"

#define GENERAL_BLOCKCHAIN 0
#define VALIDATOR_BLOCKCHAIN 1
#define CURRENT_CHUNK 0

#define BLOCK_DATA_SIZE (SHA384_DIGEST_LENGTH * 2 + 1) + sizeof(size_t) + sizeof(uint16_t) + sizeof(time_t)
#define BLOCK_SIZE 2048 + sizeof(size_t) + BLOCK_DATA_SIZE + SHA384_DIGEST_LENGTH * 2 + 1

#define MAX_TRANSACTIONS_PER_BLOCK 0x4000
#define NB_BLOCK_PER_CHUNK 10000

// Standard implementation
typedef struct BlockData
{
    char previous_block_hash[SHA384_DIGEST_LENGTH * 2 + 1]; // Previous block SHA384 hash
    size_t height;                                          // The height of the block inside the blockchain

    uint16_t nb_transactions;   // The number of validated transactions inside the current block
    Transaction **transactions; // Transactions vector

    //* Validator area
    RSA *validator_public_key; // The public key of the validator
    time_t block_timestamp;    // The block creation timestamp
} BlockData;

typedef struct Block
{
    uint16_t chunk_id;
    BlockData block_data; // The block distributed data, excluding the block signature
    
    size_t signature_len;  // The length of the signature
    char *block_signature; // SHA384 signature

} Block;

typedef struct ChunkBlockchain
{
    char blockchain_flag; // GENERAL_BLOCKCHAIN or VALIDATOR_BLOCKCHAIN
    size_t chunk_nb;      // The split offset
    Block **chunk;        // The splited blocks
} ChunkBlockchain;

/**
 * @brief Load a blockchain object with a padding of 'nb_chunk'
 * 
 * @param nb_chunk The chunk nb, 
 * if 0 : return the current blockchain object without modification
 * @param blockchain_flag The blockchain flag
 *  GENERAL_BLOCKCHAIN or VALIDATOR_BLOCKCHAIN
 * @return ChunkBlockchain*, NULL if the ChunkBlockchain is empty after switching
 */
ChunkBlockchain *get_blockchain(size_t nb_chunk, char blockchain_flag);
/**
 * @brief Write struct block to file
 * 
 * @param block The block to write
 */
void write_block_file(Block block, char blockchain);

Block *get_block(size_t block_height, char blockchain);

/**
 * @brief Free a struct block
 * 
 * @param block The block to free
 */
void free_block(Block *block);

/**
 * @brief For a block of height `h`, return the block of height `h+1`
 * 
 * @param block The base block
 * @param blockchain The blockchain flag (GENERAL_BLOCKCHAIN or VALIDATOR_BLOCKCHAIN)
 * @return The next Block* 
 */
Block *get_next_block(Block *block, char blockchain);

/**
 * @brief For a block of height `h`, return the block of height `h-1`
 * 
 * @param block The base block
 * @param blockchain The blockchain flag (GENERAL_BLOCKCHAIN or VALIDATOR_BLOCKCHAIN)
 * @return The next Block* 
 */
Block *get_prev_block(Block *block, char blockchain);
#endif