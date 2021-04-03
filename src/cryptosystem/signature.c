#include "core/blockchain/block.h"
#include "cryptosystem/signature.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>

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
void convert_transactiondata_to_data(TransactionData *transaction, int fd)
{
    BIO *pubkey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey, transaction->sender_public_key);
    int rsa_size = BIO_pending(pubkey);
    write(fd, &rsa_size, sizeof(int));
    char temp[1000];
    BIO_read(pubkey, temp, rsa_size);
    write(fd, temp, rsa_size);

    PEM_write_bio_RSAPublicKey(pubkey, transaction->receiver_public_key);
    rsa_size = BIO_pending(pubkey);
    write(fd, &rsa_size, sizeof(int));
    BIO_read(pubkey, temp, rsa_size);
    write(fd, temp, rsa_size);

    PEM_write_bio_RSAPublicKey(pubkey, transaction->organisation_public_key);
    rsa_size = BIO_pending(pubkey);
    write(fd, &rsa_size, sizeof(int));
    BIO_read(pubkey, temp, rsa_size);
    write(fd, temp, rsa_size);

    BIO_free_all(pubkey);

    write(fd, &transaction->amount, sizeof(size_t));
    write(fd, &transaction->transaction_timestamp, sizeof(time_t));
    write(fd, transaction->cause, 512);
    write(fd, transaction->asset, 512);
}

void convert_transaction_to_data(Transaction *transaction, int fd)
{
    convert_transactiondata_to_data(&transaction->transaction_data, fd);
    write(fd, &transaction->signature_len, sizeof(size_t));
    write(fd, transaction->transaction_signature, transaction->signature_len);
}

char *convert_transactions_to_data(Transaction *transactions, size_t nb_trans, int fd)
{
    char *buffer = NULL;
    size_t index = 0;
    for (size_t i = 0; i < nb_trans; i++)
    {
        convert_transaction_to_data(transactions + i, fd);
    }
    return buffer;
}

void convert_blockdata_to_data(BlockData blockdata, int fd)
{
    // IGNORE BLOCK PREV AND NEXT BECAUSE RECUP ADDR AT PARSING
    write(fd, blockdata.previous_block_hash, 97);
    write(fd, &blockdata.height, sizeof(size_t));
    write(fd, &blockdata.nb_transactions, sizeof(uint16_t));

    BIO *pubkey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey, blockdata.validator_public_key);
    int rsa_size = BIO_pending(pubkey);
    write(fd, &rsa_size, sizeof(int));
    char temp[1000];
    BIO_read(pubkey, temp, rsa_size);
    write(fd, &temp, rsa_size);
    
    write(fd, &blockdata.block_timestamp, sizeof(time_t));
    for (size_t i = 0; i < blockdata.nb_transactions; i++)
    {
        convert_transaction_to_data(blockdata.transactions + i, fd);
    }

    BIO_free_all(pubkey);
}

void convert_block_to_data(Block block, int fd)
{
    // IGNORE BLOCK PREV AND NEXT BECAUSE RECUP ADDR AT PARSING
    write(fd, block.next_block_hash, 97);
    write(fd, &block.signature_len, sizeof(size_t));
    write(fd, block.block_signature, block.signature_len);
    convert_blockdata_to_data(block.block_data, fd);
}
/*
int verify_block_signature(Block block)
{
    size_t size = 0;
    char *buf;
    convert_blockdata_to_data(block, &size, &buf);
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
    char *buff;
    convert_blockdata_to_data(*block, &size, &buff);
    block->block_signature = sign_message(buff, size, &block->signature_len);
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
*/