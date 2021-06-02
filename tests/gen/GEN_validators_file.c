#ifndef GEN_VALIDATORS_FILE_H
#define GEN_VALIDATORS_FILE_H
#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "tests_macros.h"
#include "validation/validators.h"

#include "cryptosystem/rsa.h"

#define NB_FAKE_VALIDATORS 10
#define str(x) #x

/**
 * @brief Generate a mock validators states file
 * 
 * @param path The path of the output file
 * 
 * 
 * @see 
 * For one stake transaction, power += amount / (block_height + 1) + amount
 * Foreach stake withdraw, power -= power * withdraw_stake / user_total_stake
 * 
 * validators states file description
 * Header : nb_validators[sizeof(size_t)], total_stake[sizeof(size_t)], block_height_validity[sizeof(size_t)] '\n'[sizeof(char)]
 * For each 'nb_validators' : validator_pkey[RSA_KEY_SIZE], user_stake[sizeof(size_t)] ,validator_power[sizeof(size_t)], '\n'[sizeof(char)]
 */
void gen_validators_file(char path[])
{
    LOG("Creating mock validators states file");
    FILE *validators_file = fopen(path, "w+");
    FILE *temp = fopen("data/.temp_validators", "w+");

    struct validators_state_header validators_state_header = {0};

    validators_state_header.nb_validators = NB_FAKE_VALIDATORS;
    validators_state_header.total_stake = 10000;
    validators_state_header.block_height_validity = 42;

    srand(time(NULL));
    fwrite(&validators_state_header.nb_validators, sizeof(size_t), 1, validators_file);
    fwrite(&validators_state_header.total_stake, sizeof(size_t), 1, validators_file);
    fwrite(&validators_state_header.block_height_validity, sizeof(size_t), 1, validators_file);

    for (size_t i = 0; i < NB_FAKE_VALIDATORS; i++)
    {
        struct validators_state_item validators_state_item = {0};
        validators_state_item.user_stake = (size_t)rand() % 1000;
        validators_state_item.validator_power = (size_t)rand() % 1000;

        // Validator pkey
        BIGNUM *E = BN_new();
        BN_dec2bn(&E, "3");
        RSA *keypair = RSA_new();
        RSA_generate_key_ex(keypair, 2048, E, NULL);
        PEM_write_RSAPublicKey(temp, keypair);
        fseek(temp, RSA_BEGIN_SIZE, SEEK_SET);
        fread(validators_state_item.validator_pkey, sizeof(char), RSA_KEY_SIZE, temp);

        fwrite(&validators_state_item, sizeof(struct validators_state_item), 1, validators_file);

        fseek(temp, 0, SEEK_SET);
    }

    fclose(temp);
    remove("data/.temp_validators");
    fclose(validators_file);
}
#endif