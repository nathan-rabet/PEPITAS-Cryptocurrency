#include "validation/epoch_man.h"

RSA *get_epoch_man_pkey(BlockData *block_data)
{
    if (block_data->epoch_id >= block_data->nb_validators)
        return NULL;
    return block_data->validators_public_keys[block_data->epoch_id];
}

char *create_vote_data(Block *block, char vote, int validator_index, size_t *data_length)
{
    char data[900];
    BIO *my_pub = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(my_pub, block->block_data.validators_public_keys[validator_index]);
    size_t index = BIO_pending(my_pub);
    memcpy(data, &index, sizeof(size_t));
    BIO_read(my_pub, data + sizeof(size_t), index);
    index += sizeof(size_t);
    memcpy(data + index, &block->block_data.height, sizeof(size_t));
    index += sizeof(size_t);
    memcpy(data + index, &block->block_data.epoch_id, sizeof(int));
    index += sizeof(int);
    memcpy(data + index, &vote, sizeof(char));
    index += sizeof(char);
    *data_length = index;
    char *real_data = malloc(index + (RSA_size(block->block_data.validators_public_keys[validator_index]) * 2) + 3);
    memcpy(real_data, data, index);
    return real_data;
}

void give_punishments_and_rewards(Block *last_block, Block *current_block)
{
    // Search the number of 1 in BITMAP
    int validation_yes = 0;
    for (int i = 0; i < last_block->block_data.nb_validators; i++)
    {
        validation_yes += read_single_bit((unsigned char *)last_block->validators_votes, i);
    }
    current_block->block_data.is_prev_block_valid = (last_block->block_data.nb_validators - validation_yes <= validation_yes) ? 1 : 0;

    // TEST LAST VALIDATORS VOTE 'REWARD/PUNISHMENT'
    for (int i = 0; i < last_block->block_data.nb_validators-1; i++)
    {
        RSA *validator = last_block->block_data.validators_public_keys[i];
        char *validator_signature = last_block->vote_signature[i];
        char validator_vote = read_single_bit((unsigned char *)last_block->validators_votes, i);

        // Verify signature validity
        size_t data_length;
        char *data = create_vote_data(last_block, validator_vote, i, &data_length);
        if (!verify_signature(data, data_length, validator_signature, validator))
        {
            free(data);
            return;
        }
        free(data);

        // APPLY REWARD/PUNISHMENT (NEW TRANSACTION)
        Transaction *new_trans = malloc(sizeof(Transaction));
        TransactionData *trans_data = &new_trans->transaction_data;
        trans_data->magic = 1;

        // I WON
        if (current_block->block_data.is_prev_block_valid == validator_vote)
        {
            trans_data->type = T_TYPE_REWARD_STAKE;
            trans_data->sender_public_key = NULL;
            trans_data->receiver_public_key = validator;
            trans_data->amount = 42 * 10e6;
            trans_data->sender_remaining_money = 0;
            trans_data->receiver_remaining_money = get_validator_stake(get_validator_id(validator)) + trans_data->amount;
            memcpy(trans_data->cause, "YOUPI", 6);
            memcpy(trans_data->asset, "YOUPI", 6);
        }

        // I LOSE
        else
        {
            trans_data->type = T_TYPE_PUNISH_STAKE;
            trans_data->sender_public_key = validator;
            trans_data->receiver_public_key = NULL;
            trans_data->amount = get_validator_stake(get_validator_id(validator)) / 2;
            trans_data->sender_remaining_money = trans_data->amount;
            trans_data->receiver_remaining_money = 0;
            memcpy(trans_data->cause, "Aie", 4);
            memcpy(trans_data->asset, "Aie", 4);
        }

        trans_data->transaction_timestamp = time(NULL);
        current_block->block_data.transactions[current_block->block_data.nb_transactions++] = new_trans;
    }
}

void add_pdt_to_block(Block *block){

    // SEARCH TRANSACTION TO ADD
    size_t nbdir = 0;
    time_t txids[MAX_TRANSACTIONS_PER_BLOCK];
    DIR *d;
    struct dirent *dir;
    d = opendir("data/pdt");
    if (d) {
        while ((dir = readdir(d)) != NULL && nbdir < MAX_TRANSACTIONS_PER_BLOCK) {
            if (dir->d_type == DT_REG)
            {
                *(txids + nbdir) = atol(dir->d_name);
                nbdir++;
            }
        }
        closedir(d);
    }

    // LOAD TRANSACTION
    Transaction **transs = malloc(sizeof(Transaction *) * nbdir);
    int ti = 0;
    for (size_t i = 0; i < nbdir; i++)
    {
        transs[ti] = load_pending_transaction(txids[ti]);
        if (transs[ti] != NULL)
            ti++;
    }

    // VERIFY TRANSACTION
    size_t nb_trans;
    Transaction **transs_valid = validate_transactions(transs, ti, &nb_trans);
    
    // COPY TRANSACTION TO BLOCK
    block->block_data.transactions = malloc(sizeof(Transaction *) * nb_trans);
    for (size_t i = 0; i < nb_trans; i++)
    {
        block->block_data.transactions[block->block_data.nb_transactions] = transs_valid[i];
        block->block_data.nb_transactions++;
    }

    // FREE    
    free(transs);
    free(transs_valid);
    
}

Block *create_epoch_block()
{
    Block *last_block;
    if (get_infos()->is_validator == 1)
    {
        last_block = get_block(get_infos()->actual_height);
    }
    else
    {
        last_block = get_epoch(0, get_infos()->actual_height+1);
    }
    if (last_block == NULL)
    {
        return NULL;
    }
    
    
    Block *new_block = calloc(1, sizeof(Block));

    // VERIF IF IN NEXT COMITTEE
    RSA** comittee = get_next_comittee(&new_block->block_data.nb_validators);

    // my RSA* to char*
    char my_pkey_string[RSA_FILE_TOTAL_SIZE];
    BIO *my_pub = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(my_pub, get_my_wallet()->pub_key);
    int rsa_size = BIO_pending(my_pub);
    BIO_read(my_pub, my_pkey_string, rsa_size);
    BIO_free(my_pub);
    for (int i = 0; i < new_block->block_data.nb_validators; i++)
    {
        RSA *v_key = comittee[i];

        // RSA* to char*
        char pkey_string[RSA_FILE_TOTAL_SIZE];
        BIO *pub = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(pub, v_key);
        int rsa_size = BIO_pending(pub);
        BIO_read(pub, pkey_string, rsa_size);
        BIO_free(pub);


        // IS EPOCH MAN
        if (strncmp(&my_pkey_string[RSA_BEGIN_SIZE], &pkey_string[RSA_BEGIN_SIZE], RSA_KEY_SIZE) == 0) {
            new_block->block_data.epoch_id = i;
            break;
        }
        else
        {
            // NOT IN COMITTEE
            if (i == new_block->block_data.nb_validators-1)
                return NULL;
        }
        
    }
    
    // LINK BLOCK
    for (int i = 0; i < new_block->block_data.nb_validators; i++)
    {
        new_block->block_data.validators_public_keys[i] = comittee[i];
    }

    // CREATE EPOCH
    new_block->block_data.magic = 1;
    new_block->block_data.height = last_block->block_data.height + 1;
    new_block->block_data.block_timestamp = time(NULL);
    RSA_public_decrypt(SIGNATURE_LEN, (unsigned char *)last_block->block_signature, (unsigned char *)new_block->block_data.previous_block_hash, last_block->block_data.validators_public_keys[last_block->block_data.epoch_id], RSA_PKCS1_PADDING);
    memcpy(new_block->block_data.prev_validators_votes, last_block->validators_votes, 64);

    give_punishments_and_rewards(last_block, new_block);

    add_pdt_to_block(new_block);

    // VERIFY LAST BLOCK
    char is_valid = 1;

    new_block->block_data.is_prev_block_valid = is_valid;

    sign_block(new_block);

    return new_block;
}