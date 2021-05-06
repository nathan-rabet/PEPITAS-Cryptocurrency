#include "validation/epoch_man.h"

RSA* get_epoch_man_pkey(BlockData *block_data) {
    return block_data->validators_public_keys[block_data->epoch_id];
}
