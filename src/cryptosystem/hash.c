#include <openssl/sha.h>
#include "cryptosystem/hash.h"
#include "core/blockchain/block.h"
#include "cryptosystem/signature.h"

char *sha384_data(void *data, size_t len_data)
{
    unsigned char hash[SHA384_DIGEST_LENGTH];
    SHA512_CTX sha384;
    SHA384_Init(&sha384);
    SHA384_Update(&sha384, data, len_data);
    SHA384_Final(hash, &sha384);

    char *output_buffer = malloc((2 * SHA384_DIGEST_LENGTH + 1) * sizeof(char));
    int i = 0;
    for (i = 0; i < SHA384_DIGEST_LENGTH; i++)
    {
        sprintf(output_buffer + (i * 2), "%02x", hash[i]);
    }
    output_buffer[2 * SHA384_DIGEST_LENGTH] = '\0';
    return output_buffer;
}

char *hash_block_transactions(Block *block)
{
    char *buf = NULL;
    size_t size = 0;
    for (size_t i = 0; i < block->block_data.nb_transactions; i++)
    {
        get_transaction_data(block->block_data.transactions[i], &buf, &size);
    }

    return sha384_data(buf, size);
}