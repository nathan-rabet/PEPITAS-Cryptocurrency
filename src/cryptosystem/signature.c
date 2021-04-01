#include "core/blockchain/block.h"
#include "cryptosystem/signature.h"
#include <string.h>

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

char *sign_message(char *data, size_t len_data, size_t *signature_len)
{
    Wallet *wallet = get_my_wallet();
    // Hash
    char *output_buffer = sha384_data(data, len_data);

    // encrypt the message
    char *encrypt = malloc(RSA_size(wallet->priv_key));
    ssize_t encrypt_len;
    char errmsg[130];
    if ((encrypt_len = RSA_private_encrypt(strlen(output_buffer) + 1, (unsigned char *)output_buffer,
                                           (unsigned char *)encrypt, wallet->priv_key, RSA_PKCS1_PADDING)) == -1)
    {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), errmsg);
        err(EXIT_FAILURE, "Error encrypting message: %s\n", errmsg);
    }
    *signature_len = encrypt_len;
#if TEST
    // Write the encrypted message to a file
    FILE *out = fopen("out.bin", "w");
    fwrite(encrypt, sizeof(*encrypt), RSA_size(wallet->priv_key), out);
    fclose(out);
#endif
    return encrypt;
}

int verify_signature(void *data, size_t data_len, char *signature, size_t signature_len, RSA *pub_key)
{
    // Hash
    char *output_buffer = sha384_data(data, data_len);

    // Decrypt the message
    char *decrypt = malloc(RSA_size(pub_key));
    char errmsg[130];

    if (RSA_public_decrypt(signature_len, (unsigned char *)signature, (unsigned char *)decrypt,
                           pub_key, RSA_PKCS1_PADDING) == -1)
    {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), errmsg);
        err(EXIT_FAILURE, "Error decrypting message: %s\n", errmsg);
    }

    return !strncmp(output_buffer, decrypt, SHA384_DIGEST_LENGTH * 2);
}
void convert_transactiondata_to_data(TransactionData *transaction, char *buffer, size_t *index)
{
    memcpy(buffer + *index, transaction->sender_public_key, RSA_size(transaction->sender_public_key));
    *index += RSA_size(transaction->sender_public_key);
    memcpy(buffer + *index, transaction->receiver_public_key, RSA_size(transaction->receiver_public_key));
    *index += RSA_size(transaction->receiver_public_key);
    memcpy(buffer + *index, transaction->organisation_public_key, RSA_size(transaction->organisation_public_key));
    *index += RSA_size(transaction->organisation_public_key);
    memcpy(buffer + *index, &transaction->amount, sizeof(size_t));
    *index += sizeof(size_t);
    memcpy(buffer + *index, &transaction->transaction_timestamp, sizeof(time_t));
    *index += sizeof(time_t);
    memcpy(buffer + *index, transaction->cause, 512);
    *index += 512;
    memcpy(buffer + *index, transaction->asset, 512);
    *index += 512;
}
void convert_transaction_to_data(Transaction *transaction, char *buffer, size_t *index)
{
    buffer = realloc(buffer, *index + TRANSACTION_SIZE + RSA_size(transaction->transaction_data.organisation_public_key) + RSA_size(transaction->transaction_data.receiver_public_key) + RSA_size(transaction->transaction_data.sender_public_key));
    convert_transactiondata_to_data(&transaction->transaction_data, buffer, index);
    memcpy(buffer + *index, &transaction->signature_len, sizeof(size_t));
    *index += sizeof(size_t);
    memcpy(buffer + *index, transaction->transaction_signature, transaction->signature_len);
    *index += transaction->signature_len;
}

char *convert_transactions_to_data(Transaction *transactions, size_t nb_trans)
{
    char *buffer = NULL;
    size_t index = 0;
    for (size_t i = 0; i < nb_trans; i++)
    {
        convert_transaction_to_data(transactions + i, buffer, &index);
    }
    return buffer;
}

char *convert_blockdata_to_data(Block block, size_t *index)
{
    // IGNORE BLOCK PREV AND NEXT BECAUSE RECUP ADDR AT PARSING
    char *blockdata = malloc(BLOCK_DATA_SIZE + RSA_size(block.block_data.validator_public_key));
    *index = 0;
    memcpy(blockdata + *index, block.block_data.previous_block_hash, 97);
    *index += 97;
    memcpy(blockdata + *index, &block.block_data.height, sizeof(size_t));
    *index += sizeof(size_t);
    memcpy(blockdata + *index, &block.block_data.nb_transactions, sizeof(uint16_t));
    *index += sizeof(uint16_t);
    memcpy(blockdata + *index, block.block_data.validator_public_key, RSA_size(block.block_data.validator_public_key));
    *index += RSA_size(block.block_data.validator_public_key);
    memcpy(blockdata + *index, &block.block_data.block_timestamp, sizeof(time_t));
    *index += sizeof(time_t);
    for (size_t i = 0; i < block.block_data.nb_transactions; i++)
    {
        convert_transaction_to_data(block.block_data.transactions + i, blockdata, index);
    }

#if TEST
    printf("Buffer: %li\nIndex:  %lu\n", BLOCK_DATA_SIZE + RSA_size(block.block_data.validator_public_key) + block.block_data.nb_transactions * (TRANSACTION_SIZE + RSA_size(block.block_data.transactions[0].transaction_data.organisation_public_key) + RSA_size(block.block_data.transactions[0].transaction_data.receiver_public_key) + RSA_size(block.block_data.transactions[0].transaction_data.sender_public_key)), *index);
#endif

    return blockdata;
}

int verify_block_signature(Block block)
{
    size_t size = 0;
    char *buf = convert_blockdata_to_data(block, &size);
    int ret = verify_signature( buf,
                                size,
                                block.block_signature,
                                block.signature_len,
                                block.block_data.validator_public_key);
    free(buf);
    return ret;
}

int verify_transaction_signature(Transaction transaction)
{
    size_t size = 0;
    char *buf = NULL;
    convert_transactiondata_to_data(&transaction.transaction_data, buf, &size);
    int ret = verify_signature(&transaction.transaction_data,
                            size,
                            transaction.transaction_signature,
                            transaction.signature_len,
                            transaction.transaction_data.sender_public_key);
    free(buf);
    return ret;
}

void sign_block(Block *block)
{
    if (block->block_signature != NULL)
    {
        printf("ERROR: Block already sign\n");
        return;
    }
    size_t size = 0;
    char *buf = convert_blockdata_to_data(*block, &size);
    block->block_signature = sign_message(buf, size, &block->signature_len);
}

void sign_transaction(Transaction *transaction)
{
    if (transaction->transaction_signature != NULL)
    {
        printf("ERROR: Transaction already sign\n");
        return;
    }
    size_t size = 0;
    char *buf = malloc(TRANSACTION_DATA_SIZE + RSA_size(transaction->transaction_data.organisation_public_key) + RSA_size(transaction->transaction_data.receiver_public_key) + RSA_size(transaction->transaction_data.sender_public_key));
    convert_transactiondata_to_data(&transaction->transaction_data, buf, &size);
    transaction->transaction_signature = sign_message(buf, size, &transaction->signature_len);
}

void sign_block_transactions(Block *block)
{
    for (size_t i = 0; i < block->block_data.nb_transactions; i++)
    {
        sign_transaction(block->block_data.transactions);
    }
}