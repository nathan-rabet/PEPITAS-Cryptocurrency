#include "core/blockchain/blockchain_header.h"
#include "core/blockchain/block.h"
#include "network/network.h"
#include <sys/stat.h>
#include <stdio.h>

void write_block_header(FILE *fd, Block *block, size_t height)
{
    fwrite(&height, sizeof(size_t), 1, fd);
    fwrite(block->block_signature, sizeof(char) * SHA384_DIGEST_LENGTH, 1, fd);
}

void gen_blockchain_header(infos_st *infos){
    struct stat st = {0};
    size_t index = 0;

    if (stat("blockchain", &st) == -1)
    {
        mkdir("blockchain", 0700);
    }
    FILE *blockchainh = fopen("blockchain/blockchainheader", "wb");
    if (blockchainh == NULL)
        return;
    Block *block = NULL;
    while (1)
    {
        block = get_block(index);
        if (block == NULL)
        {
            break;
        }
        write_block_header(blockchainh, block, index);
        free_block(block);
        index++;
    }
    fclose(blockchainh);
    printf("Blockchain header file generated with %lu blocks!\n", index);
    infos->actual_height = index;
}