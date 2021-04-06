#include "core/validation/validations.h"
#include "core/blockchain/block.h"
#include "cryptosystem/signature.h"
#include "cryptosystem/hash.h"
#include "misc/files.h"
#include <string.h>

#define NB_VALIDATOR_PER_BLOCK 50

RSA *get_next_validator(char blockchain_flag)
{
    // TODO
    // Get "random" value
    char *last_block_file_path = NULL;
    switch (blockchain_flag)
    {
    case GENERAL_BLOCKCHAIN:
        last_block_file_path = last_file_in_folder(".general");
        break;
    case VALIDATOR_BLOCKCHAIN:
        last_block_file_path = last_file_in_folder(".validator");
        break;
    }
    if (last_block_file_path == NULL)
        return NULL;

    //Block *last_block = get_block((size_t)atol(&last_block_file_path[5]), blockchain_flag); // [5] because filename is "blockXXX"
    //char * sha384 = hash_block_transactions(last_block);


    // For 1 validator => Stake * nb_transactions
    // Need to build a file on crafting
    return NULL;
}