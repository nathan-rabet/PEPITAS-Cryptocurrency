#include <unistd.h>
#include <openssl/sha.h>

#include "tests_macros.h"
#include "core/cryptosystem/coding.h"
#include "core/cryptosystem/rsa.h"
#include "core/blockchain/wallet.h"
#include "misc/safe.h"
#include <fcntl.h>

void generate_key_test()
{
    DEBUG();
    generate_key();

    if (access(".keys/rsa.pub", F_OK) == 0 && access(".keys/rsa", F_OK) == 0)
    {
        TEST_PASSED("Generate private/public keys");
    }
    else
    {
        TEST_FAILED("Generate private/public keys", "The files .keys/rsa and .keys/rsa.pub were not generated");
    }
}

#if 1
void get_keys_test()
{
    DEBUG();

    generate_key();
    EVP_PKEY *keys_before = EVP_PKEY_new();
    EVP_PKEY *keys_after = EVP_PKEY_new();










    unsigned char pub1[0x11F11] = {0};
    unsigned char priv1[0x11F11] = {0};
    unsigned char pub2[0x11F11] = {0};
    unsigned char priv2[0x11F11] = {0};

    FILE * private_fd = fopen("~test/rsa", "w+");
    FILE * public_fd = fopen("~test/rsa.pub", "w+");

    if (PEM_write_RSAPrivateKey(private_fd, get_my_wallet()->keypair, NULL,0,0,NULL,NULL) == 1)
        errx(EXIT_FAILURE,"Failed to write private key in file './test/rsa'");
    if (PEM_write_RSAPublicKey(public_fd, get_my_wallet()->keypair) == 1)
        errx(EXIT_FAILURE,"Failed to write public key in file './test/rsa.pub'");

    safe_read(private_fd,&priv1,0x11F11);
    safe_read(public_fd,&pub1,0x11F11);
    fclose(private_fd);
    fclose(public_fd);
    
    // Next key comp
    get_keys();
    
    private_fd = fopen("~test/rsa_get", "wr");
    public_fd = fopen("~test/rsa_get.pub", "wr");

    if (PEM_write_RSAPrivateKey(private_fd, get_my_wallet()->keypair, NULL,NULL,NULL,NULL,NULL) == 1)
        errx(EXIT_FAILURE,"Failed to write private key in file './test/rsa_get'");
    if (PEM_write_RSAPublicKey(public_fd, get_my_wallet()->keypair) == 1)
        errx(EXIT_FAILURE,"Failed to write public key in file './test/rsa_get.pub'");

    safe_read(private_fd,&priv2,0x11F11);
    safe_read(public_fd,&pub2,0x11F11);
    fclose(private_fd);
    fclose(public_fd);

    if (strncmp(pub1, pub2,0x11F11) == 0 && strncmp(priv1, priv2,0x11F11) == 0)
    {
        TEST_PASSED("GET : Check private/public keys equality");
    }
    else
        TEST_FAILED("GET : Check private/public keys equality", "The keys before and after are not equal");
}

#else
void get_keys_test(){
    DEBUG();

    generate_key();
    unsigned char pub1[0x11F11] = {0};
    unsigned char priv1[0x11F11] = {0};
    unsigned char pub2[0x11F11] = {0};
    unsigned char priv2[0x11F11] = {0};

    FILE * private_fd = fopen("~test/rsa", "w+");
    FILE * public_fd = fopen("~test/rsa.pub", "w+");

    if (PEM_write_RSAPrivateKey(private_fd, get_my_wallet()->keypair, NULL,0,0,NULL,NULL) == 1)
        errx(EXIT_FAILURE,"Failed to write private key in file './test/rsa'");
    if (PEM_write_RSAPublicKey(public_fd, get_my_wallet()->keypair) == 1)
        errx(EXIT_FAILURE,"Failed to write public key in file './test/rsa.pub'");

    safe_read(private_fd,&priv1,0x11F11);
    safe_read(public_fd,&pub1,0x11F11);
    fclose(private_fd);
    fclose(public_fd);
    
    // Next key comp
    get_keys();

    private_fd = fopen("~test/rsa_get", "wr");
    public_fd = fopen("~test/rsa_get.pub", "wr");

    if (PEM_write_RSAPrivateKey(private_fd, get_my_wallet()->keypair, NULL,NULL,NULL,NULL,NULL) == 1)
        errx(EXIT_FAILURE,"Failed to write private key in file './test/rsa_get'");
    if (PEM_write_RSAPublicKey(public_fd, get_my_wallet()->keypair) == 1)
        errx(EXIT_FAILURE,"Failed to write public key in file './test/rsa_get.pub'");

}
#endif