#include "validation/validators.h"

#define NB_RSA_CHUNK 2048 / 64

int define_nb_validators(size_t n)
{
    if (n <= 1)
        return n;

    size_t sqroot = 1;
    while (sqroot * sqroot < n)
        sqroot++;

    sqroot = 10 * (sqroot - 1);
    if (sqroot > n)
        return n;
    return MAX(sqroot, 1000);
}

char *hash_block_transactions_epoch(Block *block)
{
    char *buf = NULL;
    size_t size = 0;
    for (size_t i = 0; i < block->block_data.nb_transactions; i++)
    {
        get_transaction_data(block->block_data.transactions[i], &buf, &size);
    }

    return sha384_data(buf, size);
}

void init_validator_state()
{
    FILE *validators_states;
    if ((validators_states = fopen("validators.state", "r")) == NULL)
    {
        validators_states = fopen("validators.state", "w+");
        char zero_buff[3 * sizeof(size_t)] = {0};
        fwrite(zero_buff, 3 * sizeof(size_t), 1, validators_states);
        fwrite("\n", sizeof(char), 1, validators_states);
    }

    fclose(validators_states);
}

RSA **get_comittee(size_t block_height, int *nb_validators)
{
    // Get "random" value
    Block *working_block = get_block(block_height); // [5] because filename is "blockXXX"
    char *sha = hash_block_transactions(working_block);
    free_block(working_block);

    // Init validators states file
    FILE *validators_states = fopen("validators.state", "r");

    if (validators_states == NULL)
        err(2, "validators.state doesn't exists, please call init_validator_state() before");

    size_t total_stake;
    size_t nb_total_validators;
    if (safe_fread(&nb_total_validators, sizeof(size_t), 1, validators_states) < 1)
        return NULL;
    if (safe_fread(&total_stake, sizeof(size_t), 1, validators_states) < 1)
        return NULL;

    while (fseek(validators_states, 3 * sizeof(size_t) + sizeof(char), SEEK_SET) != 0)
        ; // escape block_height_validity[sizeof(size_t)] '\n'[sizeof(char)]

    // Deterministic algorithm for setting the committee size
    *nb_validators = define_nb_validators(nb_total_validators);
    RSA **rsa_keys = malloc(*nb_validators * sizeof(RSA *));
    size_t *already_selected_validators_index = malloc(*nb_validators * sizeof(int));

    size_t i = 0;
    size_t j = 1;
    for (size_t v = 0; v < nb_total_validators; v++)
    {
        size_t random_offset = 3 * sizeof(size_t) + sizeof(char) + (((size_t *)sha)[i] % nb_total_validators) * (RSA_KEY_SIZE + 2 * sizeof(size_t) + sizeof(char));
        while (fseek(validators_states, random_offset, SEEK_SET) != 0)
            ;                                                   // Setting random starting point
        size_t random_power = ((size_t *)sha)[j] % total_stake; // Setting random power value

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
                    ; // escape nb_total_validators[sizeof(size_t)], total_stake[sizeof(size_t)], '\n'[sizeof(char)]
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
            if ((rsa_keys[v] = PEM_read_bio_RSAPublicKey(bufio, NULL, 0, NULL)) == NULL)
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

RSA **get_next_comittee(int *nb_validators)
{
    return get_comittee(get_last_block_height(), nb_validators);
}

ssize_t get_validators_states_total_stake()
{
    FILE *validators_states = fopen("validators.state", "r");
    if (validators_states == NULL)
        err(2, "validators.state doesn't exists, please call init_validator_state() before");

    size_t total_stake;

    while (fseek(validators_states, sizeof(size_t), SEEK_SET) != 0)
        ;
    if (safe_fread(&total_stake, sizeof(size_t), 1, validators_states) < 1)
        return -1;

    fclose(validators_states);
    return total_stake;
}

ssize_t get_validators_states_nb_validators()
{
    FILE *validators_states = fopen("validators.state", "r");

    if (validators_states == NULL)
        err(2, "validators.state doesn't exists, please call init_validator_state() before");

    size_t nb_total_validators;
    if (safe_fread(&nb_total_validators, sizeof(size_t), 1, validators_states) < 1)
        return -1;
    fclose(validators_states);
    return nb_total_validators;
}

ssize_t get_validators_states_block_height_validity()
{
    FILE *validators_states = fopen("validators.state", "r");

    if (validators_states == NULL)
        err(2, "validators.state doesn't exists, please call init_validator_state() before");

    size_t block_height_validity;

    while (fseek(validators_states, 2 * sizeof(size_t), SEEK_SET) != 0)
        if (safe_fread(&block_height_validity, sizeof(size_t), 1, validators_states) < 1)
            return -1;
    fclose(validators_states);
    return block_height_validity;
}

ssize_t get_validator_stake(size_t validator_id)
{
    FILE *validators_states = fopen("validators.state", "r");

    if (validators_states == NULL)
        err(2, "validators.state doesn't exists, please call init_validator_state() before");

    size_t validator_stake;

    while (fseek(validators_states, 3 * sizeof(size_t) + sizeof(char) + (RSA_KEY_SIZE + 2 * sizeof(size_t) + sizeof(char)) * validator_id + RSA_KEY_SIZE, SEEK_SET) != 0)
        if (safe_fread(&validator_stake, sizeof(size_t), 1, validators_states) < 1)
            return -1;
    fclose(validators_states);
    return validator_stake;
}

ssize_t get_validator_power(size_t validator_id)
{
    FILE *validators_states = fopen("validators.state", "r");
    size_t validator_power;

    while (fseek(validators_states, 3 * sizeof(size_t) + sizeof(char) + (RSA_KEY_SIZE + 2 * sizeof(size_t) + sizeof(char)) * validator_id + RSA_KEY_SIZE + sizeof(size_t), SEEK_SET) != 0)
        if (safe_fread(&validator_power, sizeof(size_t), 1, validators_states) < 1)
            return -1;
    fclose(validators_states);
    return validator_power;
}

RSA *get_validator_pkey(size_t validator_id)
{
    FILE *validators_states = fopen("validators.state", "r");
    char pkey[RSA_FILE_TOTAL_SIZE + 1];

    while (fseek(validators_states, 3 * sizeof(size_t) + sizeof(char) + (RSA_KEY_SIZE + 2 * sizeof(size_t) + sizeof(char)) * validator_id, SEEK_SET) != 0)
        if (safe_fread(&pkey[RSA_BEGIN_SIZE], RSA_KEY_SIZE, 1, validators_states) < 1)
            return NULL;
    strcpy(pkey, "-----BEGIN RSA PUBLIC KEY-----\n");
    strncpy(&pkey[RSA_FILE_TOTAL_SIZE - RSA_END_SIZE], "-----END RSA PUBLIC KEY-----\n", RSA_END_SIZE + 1);

    BIO *bufio;
    RSA *rsa_pkey;
    bufio = BIO_new_mem_buf((void *)pkey, RSA_FILE_TOTAL_SIZE);
    if ((rsa_pkey = PEM_read_bio_RSAPublicKey(bufio, NULL, 0, NULL)) == NULL)
        errx(EXIT_FAILURE, "PEM_read_bio_RSAPublicKey returned NULL");

    fclose(validators_states);
    return rsa_pkey;
}

ssize_t get_validator_id(RSA *pkey)
{

    // RSA* to char*
    char pkey_string[RSA_FILE_TOTAL_SIZE];
    BIO *pubkey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey, pkey);
    int rsa_size = BIO_pending(pubkey);
    BIO_read(pubkey, pkey_string, rsa_size);
    BIO_free(pubkey);

    FILE *validators_states = fopen("validators.state", "r");
    ssize_t nb_validators;
    if (safe_fread(&nb_validators, sizeof(ssize_t), 1, validators_states) == -1)
        return -1;

    while (fseek(validators_states, 2 * sizeof(size_t) + sizeof(char), SEEK_SET) != 0)
        ;

    for (ssize_t index = 0; index < nb_validators; index++)
    {
        char temp_pkey[RSA_KEY_SIZE] = {0};

        if (safe_fread(temp_pkey, sizeof(char), RSA_KEY_SIZE, validators_states) == -1)
            return -1;
        if (strncmp(pkey_string + RSA_BEGIN_SIZE + 1, temp_pkey, RSA_KEY_SIZE) == 0)
            return index;
        while (fseek(validators_states, 2 * sizeof(size_t) + sizeof(char), SEEK_CUR) != 0)
            ;
    }

    return -1;
}

int is_commitee_member()
{
    int nb_validators;
    int id = -1;
    RSA **keys = get_next_comittee(&nb_validators);
    RSA *my_key = get_my_wallet()->pub_key;
    for (int i = 0; i < nb_validators; i++)
    {
        if (id == -1 && cmp_public_keys(keys[i], my_key))
            id = i;
        RSA_free(keys[i]);
    }
    return id;
}
