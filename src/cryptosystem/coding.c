#include "core/cryptosystem/coding.h"

void sha256_string(char *string, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for (i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

void sign_message(char *msg, char **signature, size_t *signature_len)
{
    Wallet *wallet = get_my_wallet();
    // Hash
    char *outputBuffer = malloc(65 * sizeof(char));
    sha256_string(msg, outputBuffer);
    printf("output buffer sha256 sm: %s\n", outputBuffer);

    // Encrypt the message
    char *encrypt = malloc(RSA_size(wallet->keypair));
    ssize_t encrypt_len;
    char *errmsg = malloc(130);
    if ((encrypt_len = RSA_private_encrypt(strlen(outputBuffer) + 1, (unsigned char *)outputBuffer,
                                           (unsigned char *)encrypt, wallet->keypair, RSA_PKCS1_PADDING)) == -1)
    {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), errmsg);
        err(EXIT_FAILURE, "Error encrypting message: %s\n", errmsg);
    }
    *signature = encrypt;
    *signature_len = encrypt_len;
    free(errmsg);
    #ifndef WRITE_TO_FILE
    // Write the encrypted message to a file
        FILE *out = fopen("out.bin", "w");
        fwrite(encrypt, sizeof(*encrypt),  RSA_size(wallet->keypair), out);
        fclose(out);
        printf("Encrypted message written to file.\n");
        free(encrypt);
    #endif
}

char verify_sign(char *msg, char *signature, size_t signature_len)
{
    Wallet *wallet = get_my_wallet();
    // Hash
    char *outputBuffer = malloc(65 * sizeof(char));
    sha256_string(msg, outputBuffer);
    printf("output buffer sha256 vs: %s\n", outputBuffer);

    // Decrypt the message
    char *decrypt = malloc(RSA_size(wallet->keypair));
    char *errmsg = malloc(130);

    if (RSA_public_decrypt(signature_len, (unsigned char *)signature, (unsigned char *)decrypt,
                            wallet->keypair, RSA_PKCS1_PADDING) == -1)
    {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), errmsg);
        err(EXIT_FAILURE, "Error decrypting message: %s\n", errmsg);
    }
    free(errmsg);

    return !strcmp(outputBuffer, decrypt);
}