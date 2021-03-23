#include "core/cryptosystem/coding.h"

void sign_message(char *msg, u_int64_t len, char **signature)
{
    Wallet *wallet = get_my_wallet();
    // Encrypt the message
    char *encrypt = malloc(RSA_size(wallet->keypair));
    int encrypt_len;
    char *err = malloc(130);
    if ((encrypt_len = RSA_private_encrypt(strlen(msg) + 1, (unsigned char *)msg,
                                          (unsigned char *)encrypt, wallet->keypair, RSA_PKCS1_PADDING)) == -1)
    {
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), err);
        fprintf(stderr, "Error encrypting message: %s\n", err);
    }
    *signature = encrypt;
    free(err);
}

void decrypt_msg(char *encoded_msg, u_int64_t len, char *msg)
{
    
    
}