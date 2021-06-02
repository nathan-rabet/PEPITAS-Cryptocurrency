#include "cryptosystem/rsa.h"
#define RSA_NUM_E 3

// PUBLIC KEY : (n, e)
// PRIVATE KEY: (n, d)

void get_keys(__attribute__((unused))char *password)
{
    Wallet *wallet = get_my_wallet();

    if (wallet->pub_key != NULL)
    {
        return;
    }
    

    struct stat st = {0};

    if (stat("data/keys", &st) == -1)
    {
        mkdir("data/keys", 0700);
    }

    FILE *rsa_public_file;
    FILE *rsa_private_file;
    // If there is no key, then generate new
    if (access("data/keys/rsa.pub", F_OK) || access("data/keys/rsa", F_OK))
    {
        BIGNUM *E = BN_new();
        BN_dec2bn(&E, "3");

        RSA *keypair = RSA_new();
        RSA_generate_key_ex(keypair, 2048, E, NULL);

        rsa_public_file = fopen("data/keys/rsa.pub", "wb");
        rsa_private_file = fopen("data/keys/rsa", "wb");

        if (!rsa_private_file || !rsa_public_file)
            err(errno, "Impossible to write 'data/keys/rsa.pub' and data/keys/rsa files");

        if (PEM_write_RSAPrivateKey(rsa_private_file, keypair, NULL, NULL, 0, NULL, NULL) == -1)
            err(errno, "Impossible to write data in 'data/keys/rsa'");
        fclose(rsa_private_file);

        if (PEM_write_RSAPublicKey(rsa_public_file, keypair) == -1)
            err(errno, "Impossible to write data in 'data/keys/rsa.pub'");
        fclose(rsa_public_file);

        wallet->priv_key = keypair;
        wallet->pub_key = keypair;
    }

    // If keys already exists
    else
    {
        rsa_public_file = fopen("data/keys/rsa.pub", "rb");
        rsa_private_file = fopen("data/keys/rsa", "rb");

        wallet->priv_key = PEM_read_RSAPrivateKey(rsa_private_file, NULL, NULL, NULL);
        wallet->pub_key = PEM_read_RSAPublicKey(rsa_public_file, NULL, NULL, NULL);
        fclose(rsa_public_file);
        fclose(rsa_private_file);
    }
}