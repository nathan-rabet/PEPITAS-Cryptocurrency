#include "tests_macros.h"
#include "core/blockchain/block.h"
#include "core/blockchain/transaction.h"

void *rand_data(size_t size)
{
    char *data = malloc(size * sizeof(char));
    if (size)
    {
        --size;
        for (size_t n = 0; n < size; n++)
        {
            char c = rand() % 254 + 1;
            data[n] = c;
        }
        data[size] = '\0';
    }
    return data;
}

void genererate_random_blocks_test(void)
{
    size_t const NB_BLOCK = 2;

    for (size_t i = 0; i < NB_BLOCK; i++)
    {
        Block *block = malloc(sizeof(Block));
        block->block_signature = (char *)rand_data(256);
        block->signature_len = 256;

        block->chunk_id = i % NB_BLOCK_PER_CHUNK;
        block->block_data.height = i;

        block->block_data.nb_transactions = 1;
        block->block_data.transactions =
            malloc(block->block_data.nb_transactions * sizeof(Transaction *));
        for (size_t j = 0; j < block->block_data.nb_transactions; j++)
        {
            block->block_data.transactions[j] = malloc(sizeof(Transaction));
            block->block_data.transactions[j]->transaction_data = malloc(sizeof(TransactionData));
            block->block_data.transactions[j]->signature_len = 2048;
            block->block_data.transactions[j]->transaction_signature = rand_data(2048);
            block->block_data.transactions[j]->transaction_data->organisation_public_key = RSA_generate_key(2048, 3, NULL, NULL);
            block->block_data.transactions[j]->transaction_data->receiver_public_key = RSA_generate_key(2048, 3, NULL, NULL);
            block->block_data.transactions[j]->transaction_data->sender_public_key = RSA_generate_key(2048, 3, NULL, NULL);
            block->block_data.transactions[j]->transaction_data->transaction_timestamp = time(NULL);
            block->block_data.transactions[j]->transaction_data->amount = 987654321;
        }

        write_block_file(*block, GENERAL_BLOCKCHAIN);
        free_block(block);
    }

    TEST_PASSED("Generating '%ld' random blocks", NB_BLOCK);
}

void block_test(void)
{
    if (get_blockchain(1, GENERAL_BLOCKCHAIN)->chunk_nb != 0)
    {
        TEST_PASSED("Loading chunk 1");
    }
    else
    {
        TEST_FAILED("Loading chunk 1", "chunk.chunk_nb == %ld", get_blockchain(0, GENERAL_BLOCKCHAIN)->chunk_nb);
    }
}