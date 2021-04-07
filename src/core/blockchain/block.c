#include "core/blockchain/block.h"
#include "cryptosystem/signature.h"
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>
#include <openssl/rsa.h>
#include <openssl/crypto.h>
#include <fcntl.h>
#include <sys/types.h>

ChunkBlockchain *get_blockchain(size_t nb_chunk)
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
        if (blockchain_chunk.chunk[i] != NULL) {
            free_block(blockchain_chunk.chunk[i]);
            blockchain_chunk.chunk[i] = NULL;
        }

        struct stat buffer;
        char path[32] = {0};
        snprintf(path, 256, "./blockchain/block%lu", (nb_chunk - 1) * NB_BLOCK_PER_CHUNK + i);
        if (stat(path, &buffer) != 0)
        {
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

void convert_data_to_transactiondata(TransactionData *transactiondata, FILE *blockfile)
{
    uint16_t RSAsize;
    fread(&RSAsize, sizeof(int), 1, blockfile);

    char temp[1000];
    fread(temp, RSAsize, 1, blockfile);
    BIO *pubkey = BIO_new(BIO_s_mem());
    BIO_write(pubkey, temp, RSAsize);
    transactiondata->sender_public_key = RSA_new();
    PEM_read_bio_RSAPublicKey(pubkey, &transactiondata->sender_public_key, NULL, NULL);
    BIO_free(pubkey);

    fread(&RSAsize, sizeof(int), 1, blockfile);
    fread(temp, RSAsize, 1, blockfile);
    BIO *pubkey1 = BIO_new(BIO_s_mem());
    BIO_write(pubkey1, temp, RSAsize);
    transactiondata->receiver_public_key = RSA_new();
    PEM_read_bio_RSAPublicKey(pubkey1, &transactiondata->receiver_public_key, NULL, NULL);
    BIO_free(pubkey1);

    fread(&RSAsize, sizeof(int), 1, blockfile);
    fread(temp, RSAsize, 1, blockfile);
    BIO *pubkey2 = BIO_new(BIO_s_mem());
    BIO_write(pubkey2, temp, RSAsize);
    transactiondata->organisation_public_key = RSA_new();
    PEM_read_bio_RSAPublicKey(pubkey2, &transactiondata->organisation_public_key, NULL, NULL);
    BIO_free(pubkey2);

    fread(&transactiondata->amount, sizeof(size_t), 1, blockfile);
    fread(&transactiondata->receiver_remaining_money, sizeof(size_t), 1, blockfile);
    fread(&transactiondata->sender_remaining_money, sizeof(size_t), 1, blockfile);
    fread(&transactiondata->transaction_timestamp, sizeof(time_t), 1, blockfile);
    fread(transactiondata->cause, 512, 1, blockfile);
    fread(transactiondata->asset, 512, 1, blockfile);
}

void convert_data_to_transaction(Transaction *transaction, FILE *blockfile)
{
    TransactionData *transdata = malloc(sizeof(TransactionData));
    convert_data_to_transactiondata(transdata, blockfile);
    transaction->transaction_data = transdata;
    fread(&transaction->signature_len, sizeof(size_t), 1, blockfile);
    transaction->transaction_signature = malloc(transaction->signature_len);
    fread(transaction->transaction_signature, transaction->signature_len, 1, blockfile);
}

void convert_data_to_blockdata(BlockData *blockdata, FILE *blockfile)
{
    fread(blockdata->previous_block_hash, 97, 1, blockfile);
    fread(&blockdata->height, sizeof(size_t), 1, blockfile);
    fread(&blockdata->nb_transactions, sizeof(uint16_t), 1, blockfile);

    uint16_t RSAsize;
    fread(&RSAsize, sizeof(int), 1, blockfile);

    char temp[1000];
    BIO *pubkey = BIO_new(BIO_s_mem());
    fread(temp, RSAsize, 1, blockfile);
    BIO_write(pubkey, temp, RSAsize);
    blockdata->validator_public_key = RSA_new();
    PEM_read_bio_RSAPublicKey(pubkey, &blockdata->validator_public_key, NULL, NULL);

    fread(&blockdata->block_timestamp, sizeof(time_t), 1, blockfile);
    blockdata->transactions = malloc(blockdata->nb_transactions * sizeof(Transaction *));
    for (size_t i = 0; i < blockdata->nb_transactions; i++)
    {
        blockdata->transactions[i] = malloc(sizeof(Transaction));
        convert_data_to_transaction(blockdata->transactions[i], blockfile);
    }
}

void convert_data_to_block(Block *block, FILE *blockfile)
{
    fread(&block->signature_len, sizeof(size_t), 1, blockfile);
    block->block_signature = malloc(block->signature_len);
    fread(block->block_signature, block->signature_len, 1, blockfile);
    convert_data_to_blockdata(&block->block_data, blockfile);
}

Block *get_block(size_t block_height)
{
    Block *block = malloc(sizeof(Block));
    FILE *blockfile;
    char dir[256];

    snprintf(dir, 256, "blockchain/block%lu", block_height);


    blockfile = fopen(dir, "rb");
    if (!blockfile)
        err(errno, "Impossible to read %s", dir);
    convert_data_to_block(block, blockfile);
    fclose(blockfile);

    return block;
}

void free_block(Block *block)
{
    free(block->block_signature);

    // Transaction
    for (size_t i = 0; i < block->block_data.nb_transactions; i++)
    {
        RSA_free(block->block_data.transactions[i]->transaction_data->organisation_public_key);
        RSA_free(block->block_data.transactions[i]->transaction_data->receiver_public_key);
        RSA_free(block->block_data.transactions[i]->transaction_data->sender_public_key);
        free(block->block_data.transactions[i]->transaction_signature);
    }
    free(block->block_data.transactions);
    free(block);
}

Block *get_next_block(Block *block)
{
    if (block->chunk_id == NB_BLOCK_PER_CHUNK - 1)
    {
        size_t next_chunk_nb = get_blockchain(0)->chunk_nb + 1;
        return get_blockchain(next_chunk_nb)->chunk[0];
    }
    return get_blockchain(CURRENT_CHUNK)->chunk[block->chunk_id + 1];
}

Block *get_prev_block(Block *block)
{
    if (block->chunk_id == 0)
    {
        size_t next_chunk_nb = get_blockchain(0)->chunk_nb - 1;
        return get_blockchain(next_chunk_nb)->chunk[NB_BLOCK_PER_CHUNK - 1];
    }
    return get_blockchain(CURRENT_CHUNK)->chunk[block->chunk_id - 1];
}