#include "core/validation/validations.h"
#include "core/blockchain/block.h"
#include "cryptosystem/signature.h"
#include "cryptosystem/rsa.h"
#include "cryptosystem/hash.h"
#include "misc/math.h"
#include "misc/files.h"
#include "misc/safe.h"
#include <string.h>
#include <math.h>
#include <openssl/bio.h>

#define NB_RSA_CHUNK 2048 / 64
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

RSA **get_next_committee(size_t *nb_validators)
{
    // Get "random" value
    char *last_block_file_path = last_file_in_folder("blockchain");
    Block *last_block = get_block((size_t)atol(&last_block_file_path[5])); // [5] because filename is "blockXXX"
    char *sha384 = hash_block_transactions(last_block);

    // Init validators states file
    FILE *validators_states = fopen("validators.state", "r");
    size_t total_stake;
    size_t nb_total_validators;
    if (safe_fread(&nb_total_validators, sizeof(size_t), 1, validators_states) < 1)
        return NULL;
    if (safe_fread(&total_stake, sizeof(size_t), 1, validators_states) < 1)
        return NULL;

    while (fseek(validators_states, 3 * sizeof(size_t) + sizeof(char), SEEK_SET) != 0)
        ; // escape block_height_validity[sizeof(size_t)] '\n'[sizeof(char)]

    // Deterministic algorithm for setting the next committee size
    *nb_validators = define_nb_validators(nb_total_validators);
    RSA **rsa_keys = malloc(*nb_validators * sizeof(RSA *));
    size_t *already_selected_validators_index = malloc(*nb_validators * sizeof(size_t));

    size_t i = 0;
    size_t j = 1;
    for (size_t v = 0; v < nb_total_validators; v++)
    {
        size_t random_offset = 3 * sizeof(size_t) + sizeof(char) + (((size_t *)sha384)[i] % nb_total_validators) * (RSA_KEY_SIZE + 2 * sizeof(size_t) + sizeof(char));
        while (fseek(validators_states, random_offset, SEEK_SET) != 0)
            ;                                                      // Setting random starting point
        size_t random_power = ((size_t *)sha384)[j] % total_stake; // Setting random power value

        char next_validator_pk[RSA_KEY_SIZE];

        char is_next_validator = 0;
        size_t current_validator = 0;
        while (!is_next_validator)
        {
            
            size_t next_validator_power;

            ssize_t rsa_fread = safe_fread(&next_validator_pk, sizeof(char), RSA_KEY_SIZE, validators_states);
            while (fseek(validators_states, sizeof(size_t), SEEK_CUR) != 0)
                ; // 'user_stake' escape

            if (rsa_fread < RSA_KEY_SIZE || safe_fread(&next_validator_power, sizeof(size_t), 1, validators_states) < 1)
            {
                while (fseek(validators_states, sizeof(size_t) * 3 + sizeof(char), SEEK_SET) != 0)
                    ; // escape nb_validators[sizeof(size_t)], total_stake[sizeof(size_t)], '\n'[sizeof(char)]
                continue;
            }
            is_next_validator = __builtin_usubl_overflow(random_power, next_validator_power, &random_power);
            while (fseek(validators_states, sizeof(char), SEEK_CUR) != 0)
                ; // '\n' escape

            current_validator++;
        }
        current_validator--;

        char alreadyExists = 0;
        for (size_t k = 0; k < v; k++)
        {
            if (already_selected_validators_index[k] == current_validator)
                alreadyExists = 1;
        }

        if (!alreadyExists)
        {
            char rsa_string[RSA_FILE_TOTAL_SIZE + 1] = {0};
            strcpy(rsa_string, "-----BEGIN RSA PUBLIC KEY-----\n");
            memcpy(&rsa_string[RSA_BEGIN_SIZE], next_validator_pk, RSA_KEY_SIZE);
            strncpy(&rsa_string[RSA_FILE_TOTAL_SIZE - RSA_END_SIZE], "-----END RSA PUBLIC KEY-----\n", RSA_END_SIZE + 1);

            BIO *bufio;
            bufio = BIO_new_mem_buf((void *)rsa_string, RSA_FILE_TOTAL_SIZE);
            if (PEM_read_bio_RSAPublicKey(bufio, &rsa_keys[v], 0, NULL) == NULL)
                errx(EXIT_FAILURE, "PEM_read_bio_RSAPublicKey returned NULL");
            already_selected_validators_index[v] = current_validator;
        }

        if (++j >= NB_RSA_CHUNK)
        {
            i++;
            j = 0;
        }
    }

    free(already_selected_validators_index);
    return rsa_keys;
}