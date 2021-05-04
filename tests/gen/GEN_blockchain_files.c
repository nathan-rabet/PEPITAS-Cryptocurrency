#ifndef GEN_BLC_F_C
#define GEN_BLC_F_C

#include "tests_macros.h"
#include "blockchain/block.h"
#include "blockchain/transaction.h"

void rand_data(size_t size,char* buffer)
{
    if (size)
    {
        --size;
        for (size_t n = 0; n < size; n++)
        {
            char c = rand() % 254 + 1;
            buffer[n] = c;
        }
        buffer[size] = '\0';
    }
}

void gen_blockchain(size_t nb_blocks)
{
    size_t const NB_BLOCK = nb_blocks;
    LOG("Creating '%ld' fake blocks", NB_BLOCK);
    for (size_t i = 0; i < NB_BLOCK; i++)
    {
        Block *block = malloc(sizeof(Block));
        rand_data(SIGNATURE_LEN,block->block_signature);

        block->chunk_id = i % NB_BLOCK_PER_CHUNK;
        block->block_data.height = i;

        block->block_data.nb_transactions = 1;
        block->block_data.transactions =
            malloc(block->block_data.nb_transactions * sizeof(Transaction *));
        for (size_t j = 0; j < block->block_data.nb_transactions; j++)
        {

            block->block_data.transactions[j] = malloc(sizeof(Transaction));
            block->block_data.transactions[j]->transaction_data = malloc(sizeof(TransactionData));
            rand_data(SIGNATURE_LEN,block->block_data.transactions[j]->transaction_signature);

            BIGNUM *E = BN_new();
            BN_dec2bn(&E, "3");

            block->block_data.transactions[j]->transaction_data->organisation_public_key = RSA_new();
            RSA_generate_key_ex(block->block_data.transactions[j]->transaction_data->organisation_public_key, 2048, E, NULL);

            block->block_data.transactions[j]->transaction_data->receiver_public_key = RSA_new();
            RSA_generate_key_ex(block->block_data.transactions[j]->transaction_data->receiver_public_key, 2048, E, NULL);

            block->block_data.transactions[j]->transaction_data->sender_public_key = RSA_new();
            RSA_generate_key_ex(block->block_data.transactions[j]->transaction_data->sender_public_key, 2048, E, NULL);

            block->block_data.transactions[j]->transaction_data->transaction_timestamp = time(NULL);
            block->block_data.transactions[j]->transaction_data->amount = 987654321;
        }

        write_block_file(*block);
        free_block(block);
    }
}
#endif