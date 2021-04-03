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

void write_block_file(Block block)
{
    struct stat st = {0};

    if (stat(".blocks", &st) == -1)
    {
        mkdir(".blocks", 0700);
    }

    int fd = open("./.blocks/block.block", O_WRONLY | O_CREAT, 0644);
    if (fd == -1)
        err(errno, "Impossible to write '.blocks/block.block'");
    convert_block_to_data(block, fd);
    close(fd);
}

void convert_data_to_transactiondata(TransactionData *transactiondata, FILE *blockfile)
{
    uint16_t RSAsize;
    fread(&RSAsize, sizeof(int), 1, blockfile);

    char temp[1000];
    BIO *pubkey = BIO_new(BIO_s_mem());
    fread(temp, RSAsize, 1, blockfile);
    BIO_write(pubkey, temp, RSAsize);
    transactiondata->sender_public_key = RSA_new();
    PEM_read_bio_RSAPublicKey(pubkey, &transactiondata->sender_public_key, NULL, NULL);

    fread(&RSAsize, sizeof(int), 1, blockfile);
    fread(temp, RSAsize, 1, blockfile);
    BIO_write(pubkey, temp, RSAsize);
    transactiondata->receiver_public_key = RSA_new();
    PEM_read_bio_RSAPublicKey(pubkey, &transactiondata->receiver_public_key, NULL, NULL);

    fread(&RSAsize, sizeof(int), 1, blockfile);
    fread(temp, RSAsize, 1, blockfile);
    BIO_write(pubkey, temp, RSAsize);
    transactiondata->organisation_public_key = RSA_new();
    PEM_read_bio_RSAPublicKey(pubkey, &transactiondata->organisation_public_key, NULL, NULL);
    
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

Block get_block()
{
    Block block;
    FILE *blockfile;
    if (!access(".blocks/block.block", F_OK))
    {
        blockfile = fopen("./.blocks/block.block", "rb");
        if (!blockfile)
            err(errno, "Impossible to read '.blocks/block.block'");
        convert_data_to_block(&block, blockfile);
        fclose(blockfile);
    }
    return block;
}