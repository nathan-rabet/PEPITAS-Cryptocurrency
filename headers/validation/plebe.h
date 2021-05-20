#ifndef PLEBE_H
#define PLEBE_H

#include "blockchain/block.h"
#include "validation/validation_engine.h"

/**
 * @brief Adhere a block, write it locally
 * 
 * @param block The block to adhere
 * @return 0 if success, 2 if need to sync error, 1 if data corrupted error
 */
int plebe_adhere_block(Block *block);

#endif
