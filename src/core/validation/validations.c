#include "core/validation/validations.h"
#include "core/blockchain/block.h"
#include "cryptosystem/signature.h"
#include "cryptosystem/hash.h"
#include "misc/math.h"
#include "misc/files.h"
#include <string.h>
#include <math.h>

#define MIN_VALIDATORS_PER_BLOCK 10
#define min(x, y) x < y ? x : y

char pseudo_log(size_t n)
{
    if (n == 0)
        return 0;

    int msb = 0;
    n = n / 2;
    while (n != 0)
    {
        n = n / 2;
        msb++;
    }

    return msb;
}

RSA **get_next_validators()
{
    // TODO
    // Get "random" value
    char *last_block_file_path = last_file_in_folder("blockchain");

    Block *last_block = get_block((size_t)atol(&last_block_file_path[5])); // [5] because filename is "blockXXX"
    char *sha384 = hash_block_transactions(last_block);

    // Need to build a file on crafting
    // Rule for node power : linear combination
    /// For one stake transaction, power += amount / block_height + amount
    /// Foreach stake withdraw, power *= withdraw_stake / total_stake
    FILE *validators_states = fopen("validators.state", "r");
    size_t nb_validators;
    if (fread(&nb_validators, sizeof(size_t), 1, validators_states <= 0))
        return NULL;

    char pdl = pseudo_log(nb_validators);
    nb_validators = MAX(MIN_VALIDATORS_PER_BLOCK, pdl);
    for (char i = 0; i < nb_validators; i++)
    {
        /* code */
    }

    return NULL;
}