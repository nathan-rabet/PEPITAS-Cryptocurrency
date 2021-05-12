#include "blockchain/blockchain_header.h"

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
    infos->actual_height = (index == 0 ? 1 : index) - 1;
    MANAGERMSG
    printf("Blockchain header file generated with %lu blocks!\n", index);
}

size_t get_receiver_remaining_money(infos_st *infos, RSA *receiver_public_key){
    size_t index = infos->actual_height;
    size_t money = 0;
    Block *block = NULL;
    while (money == 0)
    {

        block = get_block(index);
        if (block == NULL)
        {
            errx(EXIT_FAILURE, "Can't load block to create a transaction in get_receiver_remaining_money");
        }
        
        for (size_t it = 0; it < block->block_data.nb_transactions; it++)
        {
            TransactionData *trans = &block->block_data.transactions[it]->transaction_data;
            if (cmp_public_keys(receiver_public_key, trans->sender_public_key))
            {
                money = trans->sender_remaining_money;
                break;
            }
            if (cmp_public_keys(receiver_public_key, trans->receiver_public_key))
            {
                money = trans->receiver_remaining_money;
                break;
            }
        }
        free_block(block);
        if (index == 0)
        {
            break;
        }
        
        index--;
    }
    return money;
}