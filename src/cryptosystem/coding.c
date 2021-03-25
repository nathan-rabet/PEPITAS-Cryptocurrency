#include "core/cryptosystem/coding.h"

void sha256_string(char *string, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

void sign_message(char *msg, char **signature)
{
    Wallet *wallet = get_my_wallet();
    // Hash
    char* outputBuffer = malloc(65 * sizeof(char));
    sha256_string(msg, outputBuffer);
    printf("output buffer sha256 sm: %s\n", outputBuffer);

    // Encrypt the message
    char *encrypt = malloc(RSA_size(wallet->keypair));
    int encrypt_len;
    char *err = malloc(130);
    if ((encrypt_len = RSA_private_encrypt(strlen(outputBuffer) + 1, (unsigned char *)outputBuffer,
                                          (unsigned char *)encrypt, wallet->keypair, RSA_PKCS1_PADDING)) == -1)
    {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error encrypting message: %s\n", err);
    }
    *signature = encrypt;
    free(err);
}

char verify_sign(char *msg, char *signature)
{
    Wallet *wallet = get_my_wallet();
    // Hash
    char* outputBuffer = malloc(65 * sizeof(char));
    sha256_string(msg, outputBuffer);
    printf("output buffer sha256 vs: %s\n", outputBuffer);
    
    // Decrypt the message
    char *decrypt = malloc(RSA_size(wallet->keypair));
    int decrypt_len;
    char *err = malloc(130);

    if ((decrypt_len = RSA_public_decrypt(strlen(signature) + 1, (unsigned char *)signature,
                                          (unsigned char *)decrypt, wallet->keypair, RSA_PKCS1_PADDING)) == -1)
    {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error decrypting message: %s\n", err);
    }
    free(err);

    return !strcmp(outputBuffer, decrypt);
}