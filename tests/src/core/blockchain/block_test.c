#include "tests_macros.h"
#include "core/blockchain/block.h"
#include "core/blockchain/transaction.h"
#include "gen/GEN_blockchain_files.c"

#undef NB_BLOCK_PER_CHUNK
#define NB_BLOCK_PER_CHUNK 10

void block_test(void)
{
    gen_blockhain(12);

    if (get_blockchain(1) != NULL && get_blockchain(0)->chunk_nb == 1)
    {
        TEST_PASSED("Loading chunk 1");
    }
    else
    {
        TEST_FAILED("Loading chunk 1", "chunk.chunk_nb == %ld", get_blockchain(0)->chunk_nb);
    }

    if (get_blockchain(2) != NULL && get_blockchain(CURRENT_CHUNK)->chunk_nb == 2 && get_blockchain(CURRENT_CHUNK)->chunk[0]->block_data.height >= NB_BLOCK_PER_CHUNK)
    {
        TEST_PASSED("Switching to next chunk");
    }
    else
    {
        TEST_FAILED("Switching to next chunk", "chunk.chunk_nb == %ld, chunk->block_data.height == %lu",
                    get_blockchain(0)->chunk_nb,
                    get_blockchain(CURRENT_CHUNK)->chunk[0]->block_data.height);
    }

    if (get_blockchain(1) != NULL && get_blockchain(CURRENT_CHUNK)->chunk_nb == 1 && get_blockchain(CURRENT_CHUNK)->chunk[0]->block_data.height < NB_BLOCK_PER_CHUNK)
    {
        TEST_PASSED("Switching to previous chunk");
    }
    else
    {
        TEST_FAILED("Switching to previous chunk", "chunk.chunk_nb == %ld, chunk->block_data.height == %lu",
                    get_blockchain(0)->chunk_nb,
                    get_blockchain(CURRENT_CHUNK)->chunk[0]->block_data.height);
    }

    // Next block
    if (get_next_block(get_blockchain(CURRENT_CHUNK)->chunk[0]) != NULL && get_blockchain(CURRENT_CHUNK)->chunk_nb == 1)
    {
        TEST_PASSED("Getting next block, without changing chunk");
    }
    else
    {
        TEST_FAILED("Getting next block, without changing chunk", "chunk.chunk_nb == %ld",
                    get_blockchain(0)->chunk_nb);
    }

    if (get_next_block(get_blockchain(CURRENT_CHUNK)->chunk[NB_BLOCK_PER_CHUNK - 1]) != NULL && get_blockchain(CURRENT_CHUNK)->chunk_nb == 2)
    {
        TEST_PASSED("Getting next block and change chunk");
    }
    else
    {
        TEST_FAILED("Getting next block and change chunk", "chunk.chunk_nb == %ld",
                    get_blockchain(0)->chunk_nb);
    }

    // Previous block
    if (get_prev_block(get_blockchain(CURRENT_CHUNK)->chunk[1]) != NULL && get_blockchain(CURRENT_CHUNK)->chunk_nb == 2)
    {
        TEST_PASSED("Getting previous block, without changing chunk");
    }
    else
    {
        TEST_FAILED("Getting previous block, without changing chunk", "chunk.chunk_nb == %ld",
                    get_blockchain(0)->chunk_nb);
    }

    if (get_prev_block(get_blockchain(CURRENT_CHUNK)->chunk[0]) != NULL && get_blockchain(CURRENT_CHUNK)->chunk_nb == 1)
    {
        TEST_PASSED("Getting previous block and change chunk");
    }
    else
    {
        TEST_FAILED("Getting previous block and change chunk", "chunk.chunk_nb == %ld",
                    get_blockchain(0)->chunk_nb);
    }
}