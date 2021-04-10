#include "core/validation/validations.h"
#include "core/blockchain/block.h"
#include "cryptosystem/signature.h"
#include "cryptosystem/hash.h"
#include "misc/math.h"
#include "misc/files.h"
#include <string.h>
#include <math.h>
#include <openssl/bio.h>

#define RSA_KEY_SIZE 365
#define RSA_FILE_TOTAL_SIZE 427
#define RSA_BEGIN_SIZE 32
#define RSA_END_SIZE 30

#define NB_RSA_CHUNK 2048 / 64
#define MAX_VALIDATORS_PER_BLOCK 10000

uint16_t define_nb_validators(size_t n)
{
    if (n <= 1)
        return n;

    uint16_t sqroot = 1;
    while (sqroot * sqroot < n)
        sqroot++;

    sqroot = 10 * (sqroot - 1);
    if (sqroot > n)
        return n;
    return MAX(sqroot, 1000);
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
    if (fread(&nb_validators, sizeof(size_t), 1, validators_states) < 1)
        return NULL;

    uint16_t def_nb_validators = define_nb_validators(nb_validators);

    uint8_t i = 0;
    uint8_t j = 1;
    RSA **rsa_keys = malloc(nb_validators * sizeof(RSA *));
    for (uint16_t v = 0; v < def_nb_validators; v++)
    {
        size_t random = ((size_t *)sha384)[i] * ((size_t *)sha384)[j];
        char is_next_validator = 0;

        char next_validator_pk[RSA_KEY_SIZE];
        size_t next_validator_power = 0;
        while (!is_next_validator)
        {
            if (fread(&next_validator_pk, sizeof(char), RSA_KEY_SIZE, validators_states) < RSA_KEY_SIZE || fread(&next_validator_power, sizeof(size_t), 1, validators_states) < 1)
            {
                fseek(validators_states, sizeof(size_t), SEEK_SET);
                continue;
            }

            is_next_validator = __builtin_usubl_overflow(random, next_validator_power, &random);
        }
        char rsa_string[RSA_FILE_TOTAL_SIZE];
        strcpy(rsa_string, "-----BEGIN RSA PUBLIC KEY-----\n");
        strncpy(&rsa_string[RSA_BEGIN_SIZE], next_validator_pk, RSA_KEY_SIZE);
        strcpy(&rsa_string[RSA_FILE_TOTAL_SIZE - RSA_END_SIZE], "-----END RSA PUBLIC KEY-----\n");

        BIO *bufio;
        bufio = BIO_new_mem_buf((void *)rsa_string, RSA_FILE_TOTAL_SIZE);
        PEM_read_bio_RSAPublicKey(bufio, &rsa_keys[v], 0, NULL);

        if (++j >= NB_RSA_CHUNK)
            i++;
        j = 0;
    }

    return rsa_keys;
}