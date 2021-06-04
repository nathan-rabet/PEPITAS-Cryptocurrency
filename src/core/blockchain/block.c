#include "blockchain/block.h"

ChunkBlockchain *load_blockchain(size_t nb_chunk)
{
#ifdef TEST
#undef NB_BLOCK_PER_CHUNK
#define NB_BLOCK_PER_CHUNK 10
#endif

    static ChunkBlockchain blockchain_chunk = {0};

    if (blockchain_chunk.chunk == NULL)
    {
        blockchain_chunk.chunk = calloc(NB_BLOCK_PER_CHUNK, sizeof(Block *));
        return load_last_blockchain();
    }
    if (nb_chunk == 0)
            return &blockchain_chunk;
    

    for (size_t i = 0; i < NB_BLOCK_PER_CHUNK; i++)
    {
        if (blockchain_chunk.chunk[i] != NULL)
        {
            free_block(blockchain_chunk.chunk[i]);
            blockchain_chunk.chunk[i] = NULL;
        }

        struct stat buffer;
        char path[60] = {0};
        snprintf(path, 60, "data/blockchain/block%lu", (nb_chunk - 1) * NB_BLOCK_PER_CHUNK + i);
        if (stat(path, &buffer))
        {
            blockchain_chunk.nb_blocks = i;
            if (i == 0)
                return NULL;
            break;
        }

        blockchain_chunk.chunk[i] = get_block((nb_chunk - 1) * NB_BLOCK_PER_CHUNK + i);
        blockchain_chunk.chunk[i]->chunk_id = i;
    }
    blockchain_chunk.chunk_nb = nb_chunk;
    return &blockchain_chunk;
}

ChunkBlockchain *load_last_blockchain()
{
    return load_blockchain((get_infos()->actual_height / NB_BLOCK_PER_CHUNK)+1);
}

void write_block_file(Block block)
{
    struct stat st = {0};
    char dir[300];

    if (stat("data/blockchain", &st) == -1)
    {
        mkdir("data/blockchain", 0700);
    }
    snprintf(dir, 300, "data/blockchain/block%lu", block.block_data.height);

    int fd = open(dir, O_WRONLY | O_CREAT, 0644);
    if (fd == -1)
        err(errno, "Impossible to write block");
    write_block(block, fd);
    close(fd);
}

void convert_data_to_blockdata(BlockData *blockdata, int fd)
{
    read(fd, &blockdata->magic, sizeof(char));
    read(fd, &blockdata->epoch_id, sizeof(int));
    read(fd, &blockdata->is_prev_block_valid, sizeof(char));
    read(fd, blockdata->previous_block_hash, 97);
    read(fd, &blockdata->height, sizeof(size_t));
    read(fd, &blockdata->nb_transactions, sizeof(uint16_t));
    read(fd, blockdata->prev_validators_votes, MAX_VALIDATORS_PER_BLOCK / 8);
    read(fd, &blockdata->nb_validators, sizeof(int));

    //Load validators
    for (int i = 0; i < blockdata->nb_validators; i++)
    {
        int RSAsize;
        read(fd, &RSAsize, sizeof(int));
        char temp[1000];
        BIO *pubkey = BIO_new(BIO_s_mem());
        read(fd, temp, RSAsize);
        BIO_write(pubkey, temp, RSAsize);
        blockdata->validators_public_keys[i] = RSA_new();
        blockdata->validators_public_keys[i] = PEM_read_bio_RSAPublicKey(pubkey, NULL, 0, NULL);
    }

    read(fd, &blockdata->block_timestamp, sizeof(time_t));
    blockdata->transactions = malloc(blockdata->nb_transactions * sizeof(Transaction *));
    for (size_t i = 0; i < blockdata->nb_transactions; i++)
    {
        blockdata->transactions[i] = malloc(sizeof(Transaction));
        load_transaction(blockdata->transactions[i], fd);
    }
}

void convert_data_to_block(Block *block, int fd)
{
    read(fd, block->block_signature, 256);
    read(fd, block->validators_votes, MAX_VALIDATORS_PER_BLOCK / 8);
    read(fd, block->vote_signature, 256 * (MAX_VALIDATORS_PER_BLOCK - 1));
    convert_data_to_blockdata(&block->block_data, fd);
}

Block *get_block(size_t block_height)
{
    Block *block = malloc(sizeof(Block));
    int blockfile;
    char dir[300];

    snprintf(dir, 300, "data/blockchain/block%lu", block_height);
    struct stat st = {0};
    if (stat(dir, &st) == -1)
    {
        return NULL;
    }

    blockfile = open(dir, O_RDONLY);
    if (blockfile == -1)
        return NULL;
    convert_data_to_block(block, blockfile);
    close(blockfile);

    return block;
}

void free_block(Block *block)
{
    clear_block(block);
    free(block);
}

Block *get_next_block(Block *block)
{
    if (block->chunk_id == NB_BLOCK_PER_CHUNK - 1)
    {
        size_t next_chunk_nb = load_blockchain(0)->chunk_nb + 1;
        return load_blockchain(next_chunk_nb)->chunk[0];
    }
    return load_blockchain(CURRENT_CHUNK)->chunk[block->chunk_id + 1];
}

Block *get_prev_block(Block *block)
{
    if (block->chunk_id == 0)
    {
        size_t next_chunk_nb = load_blockchain(0)->chunk_nb - 1;
        return load_blockchain(next_chunk_nb)->chunk[NB_BLOCK_PER_CHUNK - 1];
    }
    return load_blockchain(CURRENT_CHUNK)->chunk[block->chunk_id - 1];
}

char *get_blockdata_data(Block *block, size_t *size)
{
    size_t index = 0;
    char *buffer = malloc(BLOCK_DATA_SIZE + 1000 * MAX_VALIDATORS_PER_BLOCK);
    memcpy(buffer + index, block->block_data.previous_block_hash, 97);
    index += 97;
    memcpy(buffer + index, &block->block_data.height, sizeof(size_t));
    index += sizeof(size_t);
    memcpy(buffer + index, &block->block_data.nb_transactions, sizeof(uint16_t));
    index += sizeof(uint16_t);
    memcpy(buffer + index, &block->block_data.nb_validators, sizeof(int));
    index += sizeof(int);

    // Read validators
    for (int i = 0; i < block->block_data.nb_validators; i++)
    {
        BIO *pubkey = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(pubkey, block->block_data.validators_public_keys[i]);
        int rsa_size = BIO_pending(pubkey);
        memcpy(buffer + index, &rsa_size, sizeof(int));
        index += sizeof(int);
        char temp[1000];
        BIO_read(pubkey, temp, rsa_size);
        memcpy(buffer + index, temp, rsa_size);
        index += rsa_size;
    }

    memcpy(buffer + index, &block->block_data.block_timestamp, sizeof(time_t));
    for (size_t i = 0; i < block->block_data.nb_transactions; i++)
    {
        get_transaction_data(block->block_data.transactions[i], &buffer, &index);
    }
    if (size != NULL)
        *size = index;
    return buffer;
}

void write_blockdata(BlockData blockdata, int fd)
{
    // IGNORE BLOCK PREV AND NEXT BECAUSE RECUP ADDR AT PARSING
    write(fd, &blockdata.magic, sizeof(char));
    write(fd, &blockdata.epoch_id, sizeof(int));
    write(fd, &blockdata.is_prev_block_valid, sizeof(char));
    write(fd, blockdata.previous_block_hash, 97);
    write(fd, &blockdata.height, sizeof(size_t));
    write(fd, &blockdata.nb_transactions, sizeof(uint16_t));
    write(fd, blockdata.prev_validators_votes, MAX_VALIDATORS_PER_BLOCK / 8);
    write(fd, &blockdata.nb_validators, sizeof(int));

    // Write validators
    for (int i = 0; i < blockdata.nb_validators; i++)
    {
        BIO *pubkey = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(pubkey, blockdata.validators_public_keys[i]);
        int rsa_size = BIO_pending(pubkey);
        write(fd, &rsa_size, sizeof(int));
        char temp[1000];
        BIO_read(pubkey, temp, rsa_size);
        write(fd, &temp, rsa_size);
        BIO_free_all(pubkey);
    }

    write(fd, &blockdata.block_timestamp, sizeof(time_t));
    for (size_t i = 0; i < blockdata.nb_transactions; i++)
    {
        write_transaction(blockdata.transactions[i], fd);
    }
}

void write_block(Block block, int fd)
{
    write(fd, block.block_signature, 256);
    write(fd, block.validators_votes, MAX_VALIDATORS_PER_BLOCK / 8);
    write(fd, block.vote_signature, 256 * (MAX_VALIDATORS_PER_BLOCK - 1));
    write_blockdata(block.block_data, fd);
}

void update_wallet_with_block(Block block) {
    Wallet *wallet = get_my_wallet();
    for (size_t i = 0; i < block.block_data.nb_transactions; i++)
    {
        Transaction *trans = block.block_data.transactions[i];
        switch (trans->transaction_data.type)
        {
        case T_TYPE_DEFAULT:
            if (cmp_public_keys(trans->transaction_data.sender_public_key, wallet->pub_key))
                remove_money_from_wallet(trans->transaction_data.amount);
            if (cmp_public_keys(trans->transaction_data.receiver_public_key, wallet->pub_key))
                add_money_to_wallet(trans->transaction_data.amount);
            break;
        case T_TYPE_ADD_STAKE:
            if (cmp_public_keys(trans->transaction_data.sender_public_key, wallet->pub_key))
            {
                add_money_to_stake(trans->transaction_data.amount);
                remove_money_from_wallet(trans->transaction_data.amount);
            }
            break;
        case T_TYPE_WITHDRAW_STAKE:
            if (cmp_public_keys(trans->transaction_data.receiver_public_key, wallet->pub_key))
            {
                remove_money_from_stake(trans->transaction_data.amount);
                add_money_to_wallet(trans->transaction_data.amount);
            }
            break;
        case T_TYPE_REWARD_STAKE:
            if (cmp_public_keys(trans->transaction_data.receiver_public_key, wallet->pub_key))
            {
                add_money_to_stake(trans->transaction_data.amount);
            }
            break;
        case T_TYPE_PUNISH_STAKE:
            if (cmp_public_keys(trans->transaction_data.sender_public_key, wallet->pub_key))
            {
                remove_money_from_stake(trans->transaction_data.amount);
            }
            break;
        default:
            break;
        }

    }
    if (wallet->stake_amount > 50000000)
    {
        get_infos()->is_validator = 1;
    }
    else
    {
        get_infos()->is_validator = 0;
    }
}

void delete_epochs(size_t height){
    char dir[300];
    struct stat st = {0};

    for (int id = 0; id < MAX_VALIDATORS_PER_BLOCK; id++)
    {
        snprintf(dir, 300, "data/epoch/epoch%luid%d", height, id);
        if (stat(dir, &st) != -1)
        {
            remove(dir);
        }
    }
    
}

Block* get_epoch(int id, size_t height){
    Block *epoch = malloc(sizeof(Block));
    int epochfile;
    char dir[300];

    snprintf(dir, 300, "data/epoch/epoch%luid%d", height, id);
    struct stat st = {0};
    if (stat(dir, &st) == -1)
    {
        return NULL;
    }

    epochfile = open(dir, O_RDONLY);
    if (epochfile == -1)
        return NULL;
    convert_data_to_block(epoch, epochfile);
    close(epochfile);

    return epoch;
}

void clear_block(Block* block){
    // Transaction
    for (size_t i = 0; i < block->block_data.nb_transactions; i++)
    {
        RSA_free(block->block_data.transactions[i]->transaction_data.receiver_public_key);
        RSA_free(block->block_data.transactions[i]->transaction_data.sender_public_key);
    }
    // Validators
    for (int i = 0; i < block->block_data.nb_validators; i++)
    {
        RSA_free(block->block_data.validators_public_keys[i]);
    }

    free(block->block_data.transactions);
}
