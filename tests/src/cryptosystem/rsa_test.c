#ifndef RSA_SIZE_C
#define RSA_SIZE_C
#include "tests_macros.h"
#include "cryptosystem/signature.h"
#include "cryptosystem/rsa.h"
#include "blockchain/wallet.h"
#include "misc/math.h"


#include <stdio.h>
#include <unistd.h>
#include <openssl/sha.h>
#include "misc/safe.h"
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>

void get_keys_test()
{
    get_keys(NULL);

    if (access(".keys/rsa.pub", F_OK) == 0 && access(".keys/rsa", F_OK) == 0 && get_my_wallet()->priv_key != NULL && get_my_wallet()->pub_key)
    {
        TEST_PASSED("Generate private/public keys");
    }
    else
    {
        TEST_FAILED("Generate private/public keys", "The files .keys/rsa and .keys/rsa.pub were not generated or were not stored in memory while execution");
    }
}

void get_keys_equality_test()
{
    struct stat st = {0};

    if (stat("./~test", &st) == -1)
    {
        mkdir("~test", 0700);
    }

    // Generating keys
    get_keys(NULL);

    FILE *rsa_generate_public_file = fopen("./~test/rsa_generate.pub", "w+");
    FILE *rsa_generate_private_file = fopen("./~test/rsa_generate", "w+");

    PEM_write_RSAPublicKey(rsa_generate_public_file, get_my_wallet()->pub_key);
    PEM_write_RSAPrivateKey(rsa_generate_private_file, get_my_wallet()->priv_key, NULL, NULL, 0, NULL, NULL);

    fseek(rsa_generate_public_file, 0, SEEK_END);
    long rsa_generate_public_file_size = ftell(rsa_generate_public_file);
    fseek(rsa_generate_public_file, 0, SEEK_SET);

    fseek(rsa_generate_private_file, 0, SEEK_END);
    long rsa_generate_private_file_size = ftell(rsa_generate_private_file);
    fseek(rsa_generate_private_file, 0, SEEK_SET);

    char *buff_public_generate = malloc(rsa_generate_public_file_size + 1);
    char *buff_private_generate = malloc(rsa_generate_private_file_size + 1);

    fread(buff_public_generate, 1, rsa_generate_public_file_size, rsa_generate_public_file);
    fread(buff_private_generate, 1, rsa_generate_private_file_size, rsa_generate_private_file);

    // Getting keys (must be equal to generated keys)
    get_keys(NULL);

    FILE *rsa_get_public_file = fopen("./~test/rsa_get.pub", "w+");
    FILE *rsa_get_private_file = fopen("./~test/rsa_get", "w+");

    PEM_write_RSAPublicKey(rsa_get_public_file, get_my_wallet()->pub_key);
    PEM_write_RSAPrivateKey(rsa_get_private_file, get_my_wallet()->priv_key, NULL, NULL, 0, NULL, NULL);

    fseek(rsa_get_public_file, 0, SEEK_END);
    long rsa_get_public_file_size = ftell(rsa_get_public_file);
    fseek(rsa_get_public_file, 0, SEEK_SET);

    fseek(rsa_get_private_file, 0, SEEK_END);
    long rsa_get_private_file_size = ftell(rsa_get_private_file);
    fseek(rsa_get_private_file, 0, SEEK_SET);

    char *buff_public_get = malloc(rsa_generate_public_file_size + 1);
    char *buff_private_get = malloc(rsa_generate_private_file_size + 1);

    fread(buff_public_get, 1, rsa_get_public_file_size, rsa_get_public_file);
    fread(buff_private_get, 1, rsa_get_private_file_size, rsa_get_private_file);

    if (strncmp(buff_public_generate, buff_public_get, MAX(rsa_generate_public_file_size, rsa_get_public_file_size)) == 0 && strncmp(buff_private_generate, buff_private_get, MAX(rsa_generate_private_file_size, rsa_get_private_file_size)) == 0)
    {
        TEST_PASSED("Generate, then get keys : equality test");
    }
    else
    {
        TEST_FAILED("Generate, then get keys : equality test", "The keys before and after are not equal");
    }
}
#endif