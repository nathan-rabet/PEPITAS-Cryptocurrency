#include "validation/validators.h"

#define NB_RSA_CHUNK 2048 / 64
#define HEADER_VALIDATORS_STATE_SIZE 3 * sizeof(size_t) + sizeof(char) + (RSA_KEY_SIZE + 2 * sizeof(size_t) + sizeof(char)) * validator_id

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

void init_validators_state()
{
    FILE *validators_states;
    if ((validators_states = fopen("data/validators.state", "r")) == NULL)
    {
        validators_states = fopen("data/validators.state", "w+");
        struct validators_state_header validators_state_header = {0};
        fwrite(&validators_state_header, sizeof(struct validators_state_header), 1, validators_states);
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
    FILE *validators_states = fopen("data/validators.state", "r");

    if (validators_states == NULL)
        err(2, "validators.state doesn't exists, please call init_validator_state() before");

    struct validators_state_header validators_state_header = {0};
    if (safe_fread(&validators_state_header, sizeof(struct validators_state_header), 1, validators_states) < 1)
        return NULL;

    while (fseek(validators_states, sizeof(struct validators_state_header), SEEK_SET))
        ;

    //

    // Deterministic algorithm for setting the committee size
    *nb_validators = define_nb_validators(validators_state_header.nb_validators);
    RSA **rsa_keys = malloc(*nb_validators * sizeof(RSA *));
    size_t *already_selected_validators_index = malloc(*nb_validators * sizeof(*nb_validators));

    size_t i = 0;
    size_t j = 1;
    for (size_t v = 0; v < validators_state_header.nb_validators; v++)
    {
        size_t random_offset = sizeof(struct validators_state_header) + (((size_t *)sha)[i] % validators_state_header.nb_validators) * sizeof(struct validators_state_item);
        if (validators_state_header.nb_validators == 1)
            random_offset = sizeof(struct validators_state_header);
        while (fseek(validators_states, random_offset, SEEK_SET))
            ;                                                                           // Setting random starting point
        size_t random_power = ((size_t *)sha)[j] % validators_state_header.total_stake; // Setting random power value

        char is_next_validator = 0;
        size_t current_validator = 0;
        struct validators_state_item next_validator;
        while (!is_next_validator)
        {
            if (safe_fread(&next_validator, sizeof(struct validators_state_item), 1, validators_states) != 1)
                break;

            is_next_validator = __builtin_usubl_overflow(random_power, next_validator.validator_power, &random_power);

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
            memcpy(&rsa_string[RSA_BEGIN_SIZE], next_validator.validator_pkey, RSA_KEY_SIZE);
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
    fclose(validators_states);
    free(already_selected_validators_index);
    return rsa_keys;
}

RSA **get_next_comittee(int *nb_validators)
{
    return get_comittee(get_infos()->actual_height, nb_validators);
}

ssize_t get_validators_states_total_stake()
{
    FILE *validators_states = fopen("data/validators.state", "r");
    if (validators_states == NULL)
        err(2, "data/validators.state doesn't exists, please call init_validator_state() before");

    struct validators_state_header validators_state_header = {0};
    if (safe_fread(&validators_state_header, sizeof(struct validators_state_header), 1, validators_states) < 1)
        return -1;

    fclose(validators_states);
    return validators_state_header.total_stake;
}

ssize_t get_validators_states_nb_validators()
{
    FILE *validators_states = fopen("data/validators.state", "r");
    if (validators_states == NULL)
        err(2, "data/validators.state doesn't exists, please call init_validator_state() before");

    struct validators_state_header validators_state_header = {0};
    if (safe_fread(&validators_state_header, sizeof(struct validators_state_header), 1, validators_states) < 1)
        return -1;

    fclose(validators_states);
    return validators_state_header.nb_validators;
}

ssize_t get_validators_states_block_height_validity()
{
    FILE *validators_states = fopen("data/validators.state", "r");
    if (validators_states == NULL)
        err(2, "data/validators.state doesn't exists, please call init_validator_state() before");

    struct validators_state_header validators_state_header = {0};
    if (safe_fread(&validators_state_header, sizeof(struct validators_state_header), 1, validators_states) < 1)
        return -1;

    fclose(validators_states);
    return validators_state_header.block_height_validity;
}

ssize_t get_validator_stake(size_t validator_id)
{
    FILE *validators_states = fopen("data/validators.state", "r");

    if (validators_states == NULL)
        err(2, "data/validators.state doesn't exists, please call init_validator_state() before");

    struct validators_state_item validators_state_item = {0};

    while (fseek(validators_states, sizeof(struct validators_state_header) + sizeof(struct validators_state_item) * validator_id, SEEK_SET))
        ;
    if (safe_fread(&validators_state_item, sizeof(struct validators_state_item), 1, validators_states) < 1)
        return -1;
    fclose(validators_states);
    return validators_state_item.user_stake;
}

ssize_t get_validator_power(size_t validator_id)
{
    FILE *validators_states = fopen("data/validators.state", "r");

    if (validators_states == NULL)
        err(2, "data/validators.state doesn't exists, please call init_validator_state() before");

    struct validators_state_item validators_state_item = {0};

    while (fseek(validators_states, sizeof(struct validators_state_header) + sizeof(struct validators_state_item) * validator_id, SEEK_SET))
        ;
    if (safe_fread(&validators_state_item, sizeof(struct validators_state_item), 1, validators_states) < 1)
        return -1;
    fclose(validators_states);
    return validators_state_item.validator_power;
}

RSA *get_validator_pkey(size_t validator_id)
{

    FILE *validators_states = fopen("data/validators.state", "r");

    if (validators_states == NULL)
        err(2, "data/validators.state doesn't exists, please call init_validator_state() before");

    struct validators_state_item validators_state_item = {0};

    while (fseek(validators_states, sizeof(struct validators_state_header) + sizeof(struct validators_state_item) * validator_id, SEEK_SET))
        ;
    if (safe_fread(&validators_state_item, sizeof(struct validators_state_item), 1, validators_states) < 1)
        return NULL;

    char rsa_full_string[RSA_FILE_TOTAL_SIZE + 1];

    strcpy(rsa_full_string, "-----BEGIN RSA PUBLIC KEY-----\n");
    strncpy(&rsa_full_string[RSA_BEGIN_SIZE], validators_state_item.validator_pkey, RSA_KEY_SIZE);
    strncpy(&rsa_full_string[RSA_FILE_TOTAL_SIZE - RSA_END_SIZE], "-----END RSA PUBLIC KEY-----\n", RSA_END_SIZE + 1);

    BIO *bufio;
    RSA *rsa_pkey;
    bufio = BIO_new_mem_buf((void *)rsa_full_string, RSA_FILE_TOTAL_SIZE);
    if ((rsa_pkey = PEM_read_bio_RSAPublicKey(bufio, NULL, 0, NULL)) == NULL)
        errx(EXIT_FAILURE, "PEM_read_bio_RSAPublicKey returned NULL");

    fclose(validators_states);
    return rsa_pkey;
}

ssize_t get_validator_id(RSA *pkey)
{

    // RSA* to char*
    FILE *validators_states = fopen("data/validators.state", "r");

    if (validators_states == NULL)
        err(2, "data/validators.state doesn't exists, please call init_validator_state() before");

    char pkey_string[RSA_FILE_TOTAL_SIZE];
    BIO *pubkey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey, pkey);
    int rsa_size = BIO_pending(pubkey);
    BIO_read(pubkey, pkey_string, rsa_size);
    BIO_free(pubkey);

    struct validators_state_header validators_state_header = {0};

    if (safe_fread(&validators_state_header, sizeof(struct validators_state_header), 1, validators_states) < 1)
        return -1;

    for (size_t index = 0; index < validators_state_header.nb_validators; index++)
    {
        struct validators_state_item validators_state_item = {0};

        if (safe_fread(&validators_state_item, sizeof(struct validators_state_item), 1, validators_states) < 1)
            return -1;
        if (strncmp(pkey_string + RSA_BEGIN_SIZE, validators_state_item.validator_pkey, RSA_KEY_SIZE) == 0)
            return (ssize_t)index;
    }
    fclose(validators_states);
    return -1;
}

int i_am_commitee_member()
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

ssize_t _create_validator_item(FILE *validators_states, struct validators_state_header *updated_validators_state_header, Transaction *transaction, bool is_key_on_sender)
{
    ssize_t validator_id;
    if (is_key_on_sender)
        validator_id = get_validator_id(transaction->transaction_data.sender_public_key);
    else
        validator_id = get_validator_id(transaction->transaction_data.receiver_public_key);

    if (validator_id == -1)
    {
        // NEED TO CREATE ITEM
        struct validators_state_item new_validators_state_item = {0};

        char temp[1000];
        BIO *pubkey = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(pubkey, transaction->transaction_data.sender_public_key);
        int rsa_size = BIO_pending(pubkey);
        BIO_read(pubkey, temp, rsa_size);
        memcpy(new_validators_state_item.validator_pkey, temp + RSA_BEGIN_SIZE, RSA_KEY_SIZE);
        BIO_free(pubkey);

        while (fseek(validators_states, 0, SEEK_END))
            ;
        fwrite(&new_validators_state_item, sizeof(struct validators_state_item), 1, validators_states);
        while (fseek(validators_states, 0, SEEK_SET))
            ;

        updated_validators_state_header->nb_validators += 1;

        validator_id = updated_validators_state_header->nb_validators - 1;
        while (fseek(validators_states, 0, SEEK_SET))
            ;
        fwrite(updated_validators_state_header, sizeof(struct validators_state_header), 1, validators_states);
    }
    return validator_id;
}

char update_validators_state(Block *block)
{
    ssize_t validators_states_block_height_validity = get_validators_states_block_height_validity();
    if (validators_states_block_height_validity == -1 || (size_t)validators_states_block_height_validity != block->block_data.height)
        return -1;

    FILE *validators_states = fopen("data/validators.state", "r+");

    if (validators_states == NULL)
        err(2, "data/validators.state doesn't exists, please call init_validator_state() before");

    struct validators_state_header updated_validators_state_header = {0};
    while (fseek(validators_states, 0, SEEK_SET))
        ;

    safe_fread(&updated_validators_state_header, sizeof(struct validators_state_header), 1, validators_states);

    for (size_t t = 0; t < block->block_data.nb_transactions; t++)
    {
        Transaction *transaction = block->block_data.transactions[t];
        ssize_t validator_id;
        struct validators_state_item validator_item;

        switch (transaction->transaction_data.type)
        {
        case T_TYPE_ADD_STAKE:
            validator_id = _create_validator_item(validators_states, &updated_validators_state_header, transaction, true);
            while (fseek(validators_states, sizeof(struct validators_state_header) + sizeof(struct validators_state_item) * validator_id, SEEK_SET))
                ;

            if (safe_fread(&validator_item, sizeof(struct validators_state_item), 1, validators_states) != 1)
                errx(errno, "`data/validator.state` corrupted");
            validator_item.validator_power += transaction->transaction_data.amount / (block->block_data.height + 1) + transaction->transaction_data.amount;
            validator_item.user_stake = transaction->transaction_data.receiver_remaining_money;
            updated_validators_state_header.total_stake += transaction->transaction_data.amount;
            break;

        case T_TYPE_WITHDRAW_STAKE:
            validator_id = _create_validator_item(validators_states, &updated_validators_state_header, transaction, false);
            while (fseek(validators_states, sizeof(struct validators_state_header) + sizeof(struct validators_state_item) * validator_id, SEEK_SET))
                ;

            if (safe_fread(&validator_item, sizeof(struct validators_state_item), 1, validators_states) != 1)
                errx(errno, "`data/validator.state` corrupted");
            validator_item.user_stake = transaction->transaction_data.sender_remaining_money;
            validator_item.validator_power -= validator_item.validator_power * transaction->transaction_data.amount / validator_item.user_stake;
            updated_validators_state_header.total_stake -= transaction->transaction_data.amount;
            break;

        case T_TYPE_REWARD_STAKE:
            validator_id = _create_validator_item(validators_states, &updated_validators_state_header, transaction, true);
            while (fseek(validators_states, sizeof(struct validators_state_header) + sizeof(struct validators_state_item) * validator_id, SEEK_SET))
                ;

            if (safe_fread(&validator_item, sizeof(struct validators_state_item), 1, validators_states) != 1)
                errx(errno, "`data/validator.state` corrupted");
            validator_item.validator_power += transaction->transaction_data.amount / (block->block_data.height + 1) + transaction->transaction_data.amount;
            validator_item.user_stake = transaction->transaction_data.receiver_remaining_money;
            updated_validators_state_header.total_stake += transaction->transaction_data.amount;
            break;

        case T_TYPE_PUNISH_STAKE:
            validator_id = _create_validator_item(validators_states, &updated_validators_state_header, transaction, false);
            while (fseek(validators_states, sizeof(struct validators_state_header) + sizeof(struct validators_state_item) * validator_id, SEEK_SET))
                ;

            if (safe_fread(&validator_item, sizeof(struct validators_state_item), 1, validators_states) != 1)
                errx(errno, "`data/validator.state` corrupted");
            validator_item.user_stake = transaction->transaction_data.sender_remaining_money;
            validator_item.validator_power -= validator_item.validator_power * transaction->transaction_data.amount / validator_item.user_stake;
            updated_validators_state_header.total_stake -= transaction->transaction_data.amount;
            break;
        }
    }

    updated_validators_state_header.block_height_validity = block->block_data.height + 1;

    while (fseek(validators_states, 0, SEEK_SET))
        ;
    fwrite(&updated_validators_state_header, sizeof(struct validators_state_header), 1, validators_states);

    fclose(validators_states);

    return 0;
}