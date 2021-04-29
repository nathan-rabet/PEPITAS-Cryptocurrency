#include "validation/epoch_man.h"

RSA* get_epoch_man_pkey(BlockData *block_data) {
    return block_data->validator_public_key[block_data->epoch_id];
}
