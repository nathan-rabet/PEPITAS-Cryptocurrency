#include <unistd.h>
#include <openssl/sha.h>

#include "tests_macros.h"
#include "core/cryptosystem/coding.h"
#include "core/cryptosystem/rsa.h"
#include "core/blockchain/wallet.h"
#include "misc/safe.h"
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>

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

void get_keys_test()
{
    DEBUG();

    struct stat st = {0};

    if (stat("./~test", &st) == -1)
    {
        mkdir("~test", 0700);
    }

    // Generating keys
    generate_key();

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
    get_keys();

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

#define MAX(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

    if (strncmp(buff_public_generate, buff_public_get, MAX(rsa_generate_public_file_size, rsa_get_public_file_size)) == 0 && strncmp(buff_private_generate, buff_private_get, MAX(rsa_generate_private_file_size, rsa_get_private_file_size)) == 0)
    {
        TEST_PASSED("GET : Check private/public keys equality");
    }
    else
        TEST_FAILED("GET : Check private/public keys equality", "The keys before and after are not equal");
}