#include "blockchain/block.h"

#define GENESIS_RSA_PUB_1 "-----BEGIN RSA PUBLIC KEY-----\nMIIBCAKCAQEAwcXVgJ6Hy9nryAmSFGpRYxLtPJ1VcI9XTbV34hniNMtztMGpwSTG\nCQ28WIWiD43qjmHxvY4Y26mLYXPjlJ2HiwneSoZcLtY+gJfObGcclpI1DSA0vE72\neTBbDz8enRbJqFWenwopKDoBjvf7nwc/fqRwD0ptLC7xwlPccRiLGdOvP/IusLY0\nLCP6A9R50H7tGsbaAQfGoHYezY8p05K6XRankb7I8wsLFdU6Ew6OghX1tq02liP4\ns5DrloSsxi1mJtW7d+vln0D/a7t2bz4jI+OMtD5M5jldGMyQpzq3D8ZJokMyh6K2\nNLwrAiqDKZiIHJTw8FZidA9/yuzlRpxNHQIBAw==\n-----END RSA PUBLIC KEY-----\n"
// GENESIS_RSA_PRIV_1
// -----BEGIN RSA PRIVATE KEY-----
// MIIEpAIBAAKCAQEAwcXVgJ6Hy9nryAmSFGpRYxLtPJ1VcI9XTbV34hniNMtztMGp
// wSTGCQ28WIWiD43qjmHxvY4Y26mLYXPjlJ2HiwneSoZcLtY+gJfObGcclpI1DSA0
// vE72eTBbDz8enRbJqFWenwopKDoBjvf7nwc/fqRwD0ptLC7xwlPccRiLGdOvP/Iu
// sLY0LCP6A9R50H7tGsbaAQfGoHYezY8p05K6XRankb7I8wsLFdU6Ew6OghX1tq02
// liP4s5DrloSsxi1mJtW7d+vln0D/a7t2bz4jI+OMtD5M5jldGMyQpzq3D8ZJokMy
// h6K2NLwrAiqDKZiIHJTw8FZidA9/yuzlRpxNHQIBAwKCAQEAgS6OVb8FMpFH2rEM
// DZw2QgyeKGjjoF+PiSOlQWaWzdz3zdZxK23ZW159kFkWtQlHCZahKQll58Zc66KX
// uGkFB1vphwQ9dI7UVbqJnZoTDwwjXhV4fYn5piA8tNS/E2SGcDkUagbGGtFWX0/9
// FK9/qcL1X4bzcsn2gY09oLsHZov17yW+4MMUGhecf2LmW4tCCCXvh2pzkwNVJR9a
// IKQHQKZ3dKuU6/5KkiwlY/syEg4yfEnQ3Q3rWBeoC4c/Dl6delSSzSglYKVAqLMC
// h0/ql6w7paYHfH08k+5L4RISLxHhd9RcKBCGrOE0qgttIsn9CuGhOOHKepYcNMuo
// yvK4EwKBgQDtWVAT+EA29FgQ5DhfXQpZJw7xMx5CcnS2rA27LdEL3LV6sD3WB8Ta
// 2iH6LYqpS6tvSyySAY3yIOpZOAqrMkcdgCpXG+syhqyfovJFIw6i0NttNa/6tTo0
// vHpzR3qMxFmpKKYeN1UW2cwr6N24U/8BfKwxCjZ+mSdR0uyKtSgcFwKBgQDQ/+l8
// Z1FfEKh+YIfA6iOw538BgsnW0XxoadJndMujn2d5sn+TiUjAYHEHz4saG1U6sRHr
// SQEljoMWTS8i/lhcbyyIKER7B5x+y7ytgTegb4XGBhVG9kNNfmyrjiUPBNHORd6K
// FDTVV54wGjunIWqLD5ZOEM00IwcDqM7dYQgc6wKBgQCeO4q3+tV5+Dq17XrqPgbm
// Ggn2IhQsTE3PHV58yTYH6Hj8dX6Or9iR5sFRc7HGMnJKMh22q7P2wJw7erHHdtoT
// qsbkvUd3BHMVF0wuF18XNeeeI8qnI3wjKFGiL6cIguZwxcQUJONkkTLH8JPQN/9W
// Ux12Bs7/EMThN0hceMVoDwKBgQCLVUZS74uUtcWpla/V8W0gmlSrrIaPNlLwRoxE
// +IfCakT7zFUNBjCAQEtaily8EjjRy2FHhgDDtFdkM3TB/uWS9MhaxYL8r72p3Shz
// q3pq9QPZWWOEpCzeVEhyXsNfWIve2T8GuCM45RQgEX0aFkcHX7mJYIjNbK9Xxd8+
// QLATRwKBgQC5y7sgIyPwU3kYcc4ygoOSdmNSnK3pakhqeJfXxJ4nP25Cj0QYgrIV
// 2UOy20aZ1zGE7nPdUX2xJ3rcYhEowR40uij751l/aMsI/4pYNd+uPxJOLmFCPRsB
// e9vPFx889bxPk2/OU2xT05AyoG8yAcTaSuzlii6FV+6cafjgfrgfow==
// -----END RSA PRIVATE KEY-----

#define GENESIS_RSA_PUB_2 "-----BEGIN RSA PUBLIC KEY-----\nMIIBCAKCAQEAsrHwAjOQzmoguFCpWTEe/3x/T6KSr7dF1zYFnCq3V3v6OQFAcyt7\nQG0q138XFasRM70Hc0k589s5nKfPYSz5MCa6iDD1IKo1qrGSyF9CPfW87DwZuLXW\nhShifhsLu+VfkbjYx5h/SGmC5WSedro3cTrex7V1BbZkCeKqRMYCgtTPucyYE4pP\nqEnFQtMVAssyaDckjpWzpwun9wsoZ3qkqAAREwNecR7i2ojyUBJ8L5ZUryqmxi4F\ngwvFLnlhAeoraWqk40L3bSdnGH1u/YV59f4/MSyVmTezI6DhFx2E3Pld/Kar5PnF\nrJSEQjtjwg+OVdGnrT46SKq8JQQlgFVZzwIBAw==\n-----END RSA PUBLIC KEY-----\n"

Block *get_genesis_block()
{
    static Block genesis_block = {0};

    if (genesis_block.block_data.nb_transactions == 0)
    {
        genesis_block.block_data.nb_transactions = 2;
        genesis_block.block_data.transactions = malloc(2 * sizeof(BlockData *));
        genesis_block.block_data.transactions[0] = malloc(sizeof(Transaction));
        genesis_block.block_data.transactions[1] = malloc(sizeof(Transaction));

        BIO *bufio;
        RSA *rsa1;
        RSA *rsa2;

        bufio = BIO_new_mem_buf(GENESIS_RSA_PUB_1, strlen(GENESIS_RSA_PUB_1));
        rsa1 = PEM_read_bio_RSAPublicKey(bufio, NULL, 0, NULL);
        BIO_free(bufio);

        bufio = BIO_new_mem_buf(GENESIS_RSA_PUB_2, strlen(GENESIS_RSA_PUB_2));
        rsa2 = PEM_read_bio_RSAPublicKey(bufio, NULL, 0, NULL);
        BIO_free(bufio);

        genesis_block.block_data.transactions[0]->transaction_data.receiver_public_key = rsa1;
        genesis_block.block_data.transactions[1]->transaction_data.receiver_public_key = rsa2;
        genesis_block.block_data.transactions[0]->transaction_data.receiver_remaining_money = 4200000000;
        genesis_block.block_data.transactions[1]->transaction_data.receiver_remaining_money = 4200000000;
        genesis_block.block_data.transactions[0]->transaction_data.transaction_timestamp = 1005458400;
        genesis_block.block_data.transactions[1]->transaction_data.transaction_timestamp = 980834400;
        write_block_file(genesis_block);
    }
    return &genesis_block;
}

ChunkBlockchain *load_blockchain(size_t nb_chunk)
{
#ifdef TEST
#undef NB_BLOCK_PER_CHUNK
#define NB_BLOCK_PER_CHUNK 10
#endif

    static ChunkBlockchain blockchain_chunk = {0};

    if (blockchain_chunk.chunk == NULL)
        blockchain_chunk.chunk = calloc(NB_BLOCK_PER_CHUNK, sizeof(Block *));

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
        char path[32] = {0};
        snprintf(path, 256, "./blockchain/block%lu", (nb_chunk - 1) * NB_BLOCK_PER_CHUNK + i);
        if (stat(path, &buffer) != 0)
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
    return load_blockchain(get_last_block_height() / NB_BLOCK_PER_CHUNK);
}

size_t get_last_block_height()
{
    char *filename = last_file_in_folder("blockchain");
    return atol(&filename[5]);
}

void write_block_file(Block block)
{
    struct stat st = {0};
    char dir[256];

    if (stat("blockchain", &st) == -1)
    {
        mkdir("blockchain", 0700);
    }
    snprintf(dir, 256, "blockchain/block%lu", block.block_data.height);

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
    char dir[256];

    snprintf(dir, 256, "blockchain/block%lu", block_height);
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
    // Transaction
    for (size_t i = 0; i < block->block_data.nb_transactions; i++)
    {
        RSA_free(block->block_data.transactions[i]->transaction_data.organisation_public_key);
        RSA_free(block->block_data.transactions[i]->transaction_data.receiver_public_key);
        RSA_free(block->block_data.transactions[i]->transaction_data.sender_public_key);
    }
    // Validators
    for (int i = 0; i < block->block_data.nb_validators; i++)
    {
        RSA_free(block->block_data.validators_public_keys[i]);
    }

    free(block->block_data.transactions);
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

        // I'AM SENDER
        if (cmp_public_keys(trans->transaction_data.sender_public_key, wallet->pub_key))
        {
            if (trans->transaction_data.type == T_TYPE_DEFAULT)
                remove_money_from_wallet(trans->transaction_data.amount);
            else
                remove_money_from_stake(trans->transaction_data.amount);
        }

        // I'AM RECEIVER
        if (cmp_public_keys(trans->transaction_data.receiver_public_key, wallet->pub_key))
        {
            if (trans->transaction_data.type == T_TYPE_DEFAULT)
                add_money_to_wallet(trans->transaction_data.amount);
            else
                add_money_to_stake(trans->transaction_data.amount);
        }


    }
    
}
