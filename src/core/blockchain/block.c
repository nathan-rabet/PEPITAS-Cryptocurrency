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

ChunkBlockchain get_blockchain(size_t nb_chunk) {
    static ChunkBlockchain blockchain = {0};

    if (nb_chunk == 0 && blockchain.chunk_nb != 0)
        return blockchain;
    
    // TODO load blockchain
    return blockchain;
}

void write_block_file(Block block, char blockchain)
{
    struct stat st = {0};
    char dir[256];

    switch (blockchain)
    {
    case VALIDATOR_BLOCKCHAIN:
        if (stat(".validator", &st) == -1)
        {
            mkdir(".validator", 0700);
        }
        snprintf(dir, 256, "./.validator/block%lu", block.block_data.height);
        break;

    case GENERAL_BLOCKCHAIN:
    default:
        if (stat(".general", &st) == -1)
        {
            mkdir(".general", 0700);
        }
        snprintf(dir, 256, "./.general/block%lu", block.block_data.height);
    
        break;
    }
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
    fread(&transactiondata->transaction_timestamp, sizeof(time_t), 1, blockfile);
    fread(transactiondata->cause, 512, 1, blockfile);
    fread(transactiondata->asset, 512, 1, blockfile);
}

void convert_data_to_transaction(Transaction *transaction, FILE *blockfile)
{
    TransactionData transdata;
    convert_data_to_transactiondata(&transdata, blockfile);
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
        Transaction trans;
        convert_data_to_transaction(&trans, blockfile);
        blockdata->transactions[i] = trans;
    }
}

void convert_data_to_block(Block *block, FILE *blockfile)
{
    fread(block->next_block_hash, 97, 1, blockfile);
    fread(&block->signature_len, sizeof(size_t), 1, blockfile);
    block->block_signature = malloc(block->signature_len);
    fread(block->block_signature, block->signature_len, 1, blockfile);
    convert_data_to_blockdata(&block->block_data, blockfile);
}

Block get_block(size_t blockheight, char blockchain)
{
    Block block;
    FILE *blockfile;
    char dir[256];
    switch (blockchain)
    {
    case VALIDATOR_BLOCKCHAIN:
        snprintf(dir, 256, "./.validator/block%lu", blockheight);
        break;
    
    case GENERAL_BLOCKCHAIN:
    default:
        snprintf(dir, 256, "./.general/block%lu", blockheight);
        break;
    }
    blockfile = fopen(dir, "rb");
    if (!blockfile)
        err(errno, "Impossible to read %s", dir);
    convert_data_to_block(&block, blockfile);
    fclose(blockfile);
    return block;
}


Block get_next_block(Block block, char blockchain)
{
    return get_block(block.block_data.height + 1, blockchain);
}
Block get_prev_block(Block block, char blockchain)
{
    return get_block(block.block_data.height - 1, blockchain);
}