#include "validation/plebe.h"

static RSA** comitee = NULL;
static int nb_validators;
static size_t comitee_height = 0;

int plebe_adhere_block(Block *block) {
    // SAVING CURRENT BLOCKCHAIN
    if (block->block_data.height != get_last_block_height() + 1)
        return 2; // NEED TO SYNC
    
    if (comitee == NULL || comitee_height < block->block_data.height + 1) {
        comitee = get_comittee(block->block_data.height + 1,&nb_validators);
        comitee_height = block->block_data.height + 1;
    }

    if (get_epoch_man_pkey(&block->block_data) != comitee[block->block_data.epoch_id])
        return 1; // Data corrupted

    // VALID
    write_block_file(*block);

    Block *previous_block = get_prev_block(block);

    
    if (block->block_data.is_prev_block_valid)
        flush_pending_transactions(previous_block->block_data.transactions, previous_block->block_data.nb_transactions);

    return 0;

    

}