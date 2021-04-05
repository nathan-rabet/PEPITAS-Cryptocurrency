#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <openssl/crypto.h>
#include <openssl/ssl3.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include "core/blockchain/wallet.h"
#include "core/blockchain/block.h"

/**
 * @brief encrypt(SHA284(msg,len_data),priv_key)
 * 
 * @param data The data to sign
 * @param len_data The length of the data
 * @param signature_len The length of the data signature
 * @return char* 
 */
char *sign_message(char *data, size_t len_data, size_t *signature_len);

/**
 * @brief Apply the SHA384 algorithm on a 'data' of size 'len_data'
 * and verifies if SHA384(data, len_data) == 'signature'
 * 
 * @param data The buffer to verify
 * @param data_len The length of the buffer
 * @param signature The signature to compare with SHA384(data, len_data) 
 * @param signature_len The length of the signature
 * @param pub_key The RSA public key used for the decryption
 * @return int 
 */
int verify_signature(void *data, size_t data_len, char *signature, size_t signature_len, RSA *pub_key);
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

/**
 * @brief Signs a block
 * 
 * @param block The block to sign
 */
void sign_block(Block *block);

/**
 * @brief Sign a transaction
 * 
 * @param transaction The transaction to sign
 */
void sign_transaction(Transaction *transaction);

/**
 * @brief Signs transactions of a block
 * 
 * @param block The block to sign
 */
void sign_block_transactions(Block *block);
#endif