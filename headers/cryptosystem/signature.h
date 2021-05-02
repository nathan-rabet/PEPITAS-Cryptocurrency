#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <openssl/crypto.h>
#include <openssl/ssl3.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include "blockchain/wallet.h"
#include "blockchain/block.h"
#include "validation/epoch_man.h"

/**
 * @brief buffer <- encrypt(SHA284(msg,len_data),wallet_priv_key)
 * 
 * If buffer == NULL, return a new allocated buffer
 * 
 * @param data The data to sign
 * @param len_data The length of the data
 * @param buffer The buffer to put signature into
 * @return char* 
 */
char *sign_message(char *data, size_t len_data, void *buffer);

/**
 * @brief encrypt(SHA284(msg,len_data),key)
 * buffer <- encrypt(SHA284(msg,len_data),key)
 * 
 * If buffer == NULL, return a new allocated buffer
 * @param data The data to sign
 * @param len_data The length of the data
 * @param key The key to use for the signature
 * @param buffer The buffer to put signature into
 * @return char* 
 */
char *sign_message_with_key(char *data, size_t len_data, RSA *key, void *buffer);

/**
 * @brief Verifies if SHA384(data) == decrypt(signature,pub_key)
 * 
 * @param data The buffer to verify
 * @param data_len The length of the buffer
 * @param signature The signature to compare with SHA384(data, len_data) 
 * @param pub_key The RSA public key used for the decryption
 * @return int 
 */
int verify_signature(void *data, size_t data_len, char *signature, RSA *pub_key);
/**
 * @brief Verifies if a block signature is valid
 * 
 * @param block The block to verify
 * @return 1 if valid, 0 otherwise  
 */
int verify_block_signature(Block block);

/**
 * @brief Verifies if a transaction signature is valid
 * 
 * @param transaction The transaction to verify
 * @return 1 if valid, 0 otherwise  
 */
int verify_transaction_signature(Transaction transaction);

/**
 * @brief Convert transactions to char * buffer
 * 
 * @param transactions The transaction array
 * @param buff The buffer that receives the transactions
 * @param size The number of transactions in the array
 *
 * @return The buffer allocated (Must be freed)
 */
void get_transaction_data(Transaction *trans, char **buff, size_t *size);

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

/**
 * @brief Signs a block
 * 
 * @param block The block to sign
 */
void sign_block(Block *block);

void sign_block_with_key(Block *block, RSA *key);

void sign_transaction(Transaction *transaction);

/**
 * @brief Sign a transaction
 * 
 * @param transaction The transaction to sign
 */
void sign_transaction_with_key(Transaction *transaction, RSA *key);

/**
 * @brief Signs transactions of a block
 * 
 * @param block The block to sign
 */
void sign_block_transactions(Block *block);
#endif