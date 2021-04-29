#include "blockchain/block.h"
#include "cryptosystem/signature.h"
#include "cryptosystem/hash.h"
#include <openssl/bio.h>
#include <openssl/rsa.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

char *sign_message(char *data, size_t len_data, void *buffer)
{
    Wallet *wallet = get_my_wallet();
    // Hash
    char *sha = sha384_data(data, len_data);

    // encrypt the message
    char *encrypt = malloc(RSA_size(wallet->priv_key) * 2);
    if (buffer == NULL)
        encrypt = malloc(RSA_size(wallet->priv_key) * 2);
    else
        encrypt = buffer;
    ssize_t encrypt_len;
    char errmsg[130];
    if ((encrypt_len = RSA_private_encrypt((2 * SHA384_DIGEST_LENGTH) + 1, (unsigned char *)sha,
                                           (unsigned char *)encrypt, wallet->priv_key, RSA_PKCS1_PADDING)) == -1)
    {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), errmsg);
        err(EXIT_FAILURE, "Error encrypting message: %s\n", errmsg);
    }
    return encrypt;
}

char *sign_message_with_key(char *data, size_t len_data, RSA *key, void *buffer)
{
    // Hash
    char *output_buffer = sha384_data(data, len_data);

    // encrypt the message
    char *encrypt = malloc(RSA_size(key) * 2);
    if (buffer == NULL)
        encrypt = malloc(RSA_size(key) * 2);
    else
        encrypt = buffer;
    ssize_t encrypt_len;
    char errmsg[130];
    if ((encrypt_len = RSA_private_encrypt((2 * SHA384_DIGEST_LENGTH) + 1, (unsigned char *)output_buffer,
                                           (unsigned char *)encrypt, key, RSA_PKCS1_PADDING)) == -1)
    {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), errmsg);
        err(EXIT_FAILURE, "Error encrypting message: %s\n", errmsg);
    }
    return encrypt;
}

int verify_signature(void *data, size_t data_len, char *signature, RSA *pub_key)
{
    // Hash
    char *output_buffer = sha384_data(data, data_len);

    // Decrypt the message
    char *decrypt = malloc(RSA_size(pub_key));
    char errmsg[130];

    if (RSA_public_decrypt(SIGNATURE_LEN, (unsigned char *)signature, (unsigned char *)decrypt,
                           pub_key, RSA_PKCS1_PADDING) == -1)
    {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), errmsg);
        err(EXIT_FAILURE, "Error decrypting message: %s\n", errmsg);
    }

    return !strncmp(output_buffer, decrypt, SHA384_DIGEST_LENGTH * 2);
}

int verify_block_signature(Block block)
{
    size_t size = 0;
    char *buf = get_blockdata_data(&block, &size);
    int ret = verify_signature(buf,
                               size,
                               block.block_signature,
                               get_epoch_man_pkey());
    free(buf);
    return ret;
}

int verify_transaction_signature(Transaction transaction)
{
    size_t size = 0;
    char *buf = NULL;
    get_transaction_data(&transaction, &buf, &size);
    int ret = verify_signature(&transaction.transaction_data,
                               size,
                               transaction.transaction_signature,
                               transaction.transaction_data->sender_public_key);
    free(buf);
    return ret;
}

void sign_block(Block *block)
{
    size_t size = 0;
    char *buff = get_blockdata_data(block, &size);
    sign_message(buff, size, block->block_signature);
}

void sign_block_with_key(Block *block, RSA *key)
{
    size_t size = 0;
    char *buff = get_blockdata_data(block, &size);
    sign_message_with_key(buff, size, key, block->block_signature);
}

void sign_transaction(Transaction *transaction)
{
    size_t size = 0;
    char *buf = NULL;
    get_transaction_data(transaction, &buf, &size);
    sign_message(buf, size, transaction->transaction_signature);
}

void sign_transaction_with_key(Transaction *transaction, RSA *key)
{
    size_t size = 0;
    char *buf = NULL;
    get_transaction_data(transaction, &buf, &size);
    sign_message_with_key(buf, size, key, transaction->transaction_signature);
}

void sign_block_transactions(Block *block)
{
    for (size_t i = 0; i < block->block_data.nb_transactions; i++)
    {
        sign_transaction(block->block_data.transactions[i]);
    }
}