#include "cryptosystem/rsa.h"
#include "core/blockchain/wallet.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <errno.h>

#define RSA_NUM_E 3

// PUBLIC KEY : (n, e)
// PRIVATE KEY: (n, d)
void get_keys()
{
    Wallet *wallet = get_my_wallet();

    struct stat st = {0};

    if (stat(".keys", &st) == -1)
    {
        mkdir(".keys", 0700);
    }

    FILE *rsa_public_file;
    FILE *rsa_private_file;
    // If there is no key, then generate new
    if (access(".keys/rsa.pub", F_OK) || access(".keys/rsa", F_OK))
    {
        RSA *keypair = RSA_generate_key(2048, 3, NULL, NULL);

        rsa_public_file = fopen("./.keys/rsa.pub", "wb");
        rsa_private_file = fopen("./.keys/rsa", "wb");

        if (!rsa_private_file || !rsa_public_file)
            err(errno, "Impossible to write '.keys/rsa.pub' and .keys/rsa files");

        if (PEM_write_RSAPrivateKey(rsa_private_file, keypair, NULL, NULL, 0, NULL, NULL) == -1)
            err(errno, "Impossible to write data in '.keys/rsa'");
        fclose(rsa_private_file);

        if (PEM_write_RSAPublicKey(rsa_public_file, keypair) == -1)
            err(errno, "Impossible to write data in '.keys/rsa.pub'");
        fclose(rsa_public_file);

        wallet->priv_key = keypair;
        wallet->pub_key = keypair;
    }

    // If keys already exists
    else
    {
        rsa_public_file = fopen(".keys/rsa.pub", "rb");
        rsa_private_file = fopen(".keys/rsa", "rb");

        Wallet *wallet = get_my_wallet();
        wallet->priv_key = PEM_read_RSAPrivateKey(rsa_private_file, NULL, NULL, NULL);
        wallet->pub_key = PEM_read_RSAPublicKey(rsa_public_file, NULL, NULL, NULL);
        fclose(rsa_public_file);
        fclose(rsa_private_file);
    }
}