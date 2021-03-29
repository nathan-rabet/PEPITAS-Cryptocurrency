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
#ifndef WRITE_TO_FILE
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

int verify_block_signature(Block block)
{
    return verify_signature(&block.block_data,
                       sizeof(BlockData),
                       block.block_signature,
                       block.signature_len,
                       block.block_data.validator_public_key);
}

int verify_transaction_signature(Transaction transaction)
{
    return verify_signature(&transaction.transaction_data,
                       sizeof(Transaction),
                       transaction.transaction_signature,
                       transaction.signature_len,
                       transaction.transaction_data.sender_public_key);
}