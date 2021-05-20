#ifndef EPOCH_MAN_H
#define EPOCH_MAN_H

#include "blockchain/transaction.h"
#include "blockchain/block.h"
#include "cryptosystem/signature.h"
#include "validation_engine.h"
#include "misc/bits.h"
#include "validators.h"
#include <openssl/rsa.h>
#include <dirent.h>

char *create_vote_data(Block *block, char vote, int validator_index, size_t *data_length);

/**
 * @brief Create a block object 
 * with the previous block hash & votes
 * 
 * @see The function create a block based on the local last block
 * 
 * @return Block*
 */
Block * create_epoch_block();


/**
 * @brief Give the pkey of the creator of a block
 * 
 * @param block_data The created block data
 * @return RSA*, NULL if the data is corrupted
 */
RSA* get_epoch_man_pkey(BlockData *block_data);

/**
 * @brief Add punishmnent and reward transactions 
 * to validators of the 'prev_block' into 'current_block'
 * 
 * @see Number of added transactions = number of validators in 'prev_block'
 * 
 * @param prev_block 
 * @param current_block 
 */
void give_punishments_and_rewards(Block *prev_block, Block *current_block);
#endif