#ifndef BLOCK_H
#define BLOCK_H

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <fcntl.h>
#include <sys/types.h>


#define MAX_VALIDATORS_PER_BLOCK 512

#define SIGNATURE_LEN 256

#define CURRENT_CHUNK 0

#define BLOCK_DATA_SIZE (SHA384_DIGEST_LENGTH * 2 + 1) + sizeof(size_t) + sizeof(uint16_t) + sizeof(time_t) + sizeof(int)
#define BLOCK_SIZE 2048 + sizeof(size_t) + BLOCK_DATA_SIZE + SHA384_DIGEST_LENGTH * 2 + 1

#define MAX_TRANSACTIONS_PER_BLOCK 16384
#define NB_BLOCK_PER_CHUNK 10000
#define NB_VOTES_BITMAP MAX_VALIDATORS_PER_BLOCK / 8

#ifndef TRANS_T
#define TRANS_T
typedef struct TransactionData
{
    char magic;
    char type;
    // All users area
    RSA *sender_public_key;          // The public key of the sender
    RSA *receiver_public_key;        // The public key of the receiver
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
    TransactionData transaction_data; // Exclude the signature

    char transaction_signature[256]; // SHA384 signature
} Transaction;

#endif

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
    int nb_validators;
    RSA *validators_public_keys[MAX_VALIDATORS_PER_BLOCK];            // The public key of the validators
    time_t block_timestamp;                                         // The block creation timestamp
    
    
    
    
    
    char prev_validators_votes[NB_VOTES_BITMAP]; // INUTILE
} BlockData;

typedef struct Block
{
    uint16_t chunk_id;  // Don't show
    BlockData block_data; // The block distributed data, excluding the block signature

    char block_signature[256]; // SHA384 signature EPOCHMAN

    //Validator Vote
    char validators_votes[NB_VOTES_BITMAP]; // BITMAP
    char vote_signature[MAX_VALIDATORS_PER_BLOCK - 1][256];
} Block;

typedef struct ChunkBlockchain
{
    size_t chunk_nb; // The split offset
    Block **chunk;   // The splited blocks
    int16_t nb_blocks;        // The number of blocks loaded in the chunk
} ChunkBlockchain;

#include "client.h"
#include "transaction.h"
#include "misc/files.h"
#include "blockchain/wallet.h"
#include "cryptosystem/rsa.h"

/**
 * @brief Loads a blockchain object with a padding of 'nb_chunk'
 * 
 * @param nb_chunk The chunk nb, 
 * if 0 : return the current blockchain object without modification
 * @return ChunkBlockchain*, NULL if the ChunkBlockchain is empty after switching
 */
ChunkBlockchain *load_blockchain(size_t nb_chunk);

/**
 * @brief Load the last local blockchain chunk
 * 
 * @param nb_chunk 
 * @return ChunkBlockchain* 
 */
ChunkBlockchain *load_last_blockchain();

/**
 * @brief Writes a block struct in a file
 * 
 * @param block The block to write
 */
void write_block_file(Block block);

/**
 * @brief Get a block object
 * 
 * @param block_height The height of the block
 * @return Block* 
 */
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
 * @return The previous Block* 
 */
Block *get_prev_block(Block *block);

/**
 * @brief Get the blockdata data object
 * 
 * @param block The block 
 * @param size The size of the block
 * @return char* 
 */
char *get_blockdata_data(Block *block, size_t *size);

/**
 * @brief Writes blockdata in a file
 * 
 * @param blockdata The blockdata to write
 * @param fd The file descriptor of the file in which the blockdata is written
 */
void write_blockdata(BlockData blockdata, int fd);

/**
 * @brief Writes a block in a file
 * 
 * @param block The block to write
 * @param fd the file descriptor of the file in which the block is written
 */
void write_block(Block block, int fd);

void convert_data_to_block(Block *block, int fd);

void update_wallet_with_block(Block block);

void delete_epochs(size_t height);

Block* get_epoch(int id, size_t height);

void clear_block(Block* block);

#endif