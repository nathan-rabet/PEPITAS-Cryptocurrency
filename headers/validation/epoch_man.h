#ifndef EPOCH_MAN_H
#define EPOCH_MAN_H

#include "blockchain/block.h"
#include "blockchain/transaction.h"

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
 * @return RSA* 
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

/**
 * @brief Delete block transactions in pdt if the block is valid
 * 
 * @param block 
 * @return 1 if the flush proceed, 0 if not
 */
int flush_pending_transactions(Block* block);
#endif