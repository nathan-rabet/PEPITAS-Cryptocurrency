#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include "blockchain/block.h"

/**
 * @brief Apply the SHA384 algorithm on a 'data' of size 'len_data'
 * 
 * @param data The buffer to hash
 * @param len_data The length of the buffer
 * @return char[97] (on heap)
 */
char *sha384_data(void *data, size_t len_data);

/**
 * @brief Apply the SHA384 to all block transactions
 * @param block The block to deal with
 * @return sha384[SHA384_DIGEST_LENGTH]
 */
char*  hash_block_transactions(Block *block);

#endif