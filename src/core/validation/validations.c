#include "core/validation/validations.h"
#include "core/blockchain/block.h"
#include "cryptosystem/signature.h"
#include "cryptosystem/rsa.h"
#include "cryptosystem/hash.h"
#include "misc/math.h"
#include "misc/files.h"
#include <string.h>
#include <math.h>
#include <openssl/bio.h>

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
    size_t total_stake;
    if (fread(&nb_validators, sizeof(size_t), 1, validators_states) < 1)
        return NULL;
    if (fread(&total_stake, sizeof(size_t), 1, validators_states) < 1)
        return NULL;

    uint16_t def_nb_validators = define_nb_validators(nb_validators);

    RSA **rsa_keys = malloc(nb_validators * sizeof(RSA *));

    // TODO : Deal with duplicates

    for (uint16_t v = 0; v < def_nb_validators; v++)
    {
        for (size_t i = 0; i < NB_RSA_CHUNK; i++)
        {
            for (size_t j = i + 1; j < NB_RSA_CHUNK; j++)
            {
                size_t random = ((size_t *)sha384)[i] * ((size_t *)sha384)[j] % total_stake;
                char is_next_validator = 0;

                char next_validator_pk[RSA_KEY_SIZE];
                size_t next_validator_power = 0;
                while (!is_next_validator)
                {
                    if (fread(&next_validator_pk, sizeof(char), RSA_KEY_SIZE, validators_states) < RSA_KEY_SIZE || fread(&next_validator_power, sizeof(size_t), 1, validators_states) < 1)
                    {
                        fseek(validators_states, sizeof(size_t) * 2, SEEK_SET);
                        continue;
                    }

                    is_next_validator = __builtin_usubl_overflow(random, next_validator_power, &random);
                }
                char rsa_string[RSA_FILE_TOTAL_SIZE + 1] = {0};
                strcpy(rsa_string, "-----BEGIN RSA PUBLIC KEY-----\n");
                memcpy(&rsa_string[RSA_BEGIN_SIZE], next_validator_pk, RSA_KEY_SIZE);
                memcpy(&rsa_string[RSA_FILE_TOTAL_SIZE - RSA_END_SIZE], "-----END RSA PUBLIC KEY-----\n", RSA_END_SIZE);

                BIO *bufio;
                bufio = BIO_new_mem_buf((void *)rsa_string, RSA_FILE_TOTAL_SIZE);
                if (PEM_read_bio_RSAPublicKey(bufio, &rsa_keys[v], 0, NULL) == NULL)
                    errx(EXIT_FAILURE, "PEM_read_bio_RSAPublicKey returned NULL");
            }
        }
    }

    return rsa_keys;
}