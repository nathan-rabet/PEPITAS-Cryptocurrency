#include "core/validation/validations.h"
#include "core/blockchain/block.h"
#include "cryptosystem/signature.h"
#include "cryptosystem/hash.h"
#include "misc/files.h"
#include <string.h>

#define NB_VALIDATOR_PER_BLOCK 50

RSA **get_next_validators()
{
    // TODO
    // Get "random" value
    char *last_block_file_path = last_file_in_folder("blockchain");

    Block *last_block = get_block((size_t)atol(&last_block_file_path[5])); // [5] because filename is "blockXXX"
    char * sha384 = hash_block_transactions(last_block);


    // Need to build a file on crafting
    FILE* validators_states = fopen("validators.state","r");

    return NULL;
}