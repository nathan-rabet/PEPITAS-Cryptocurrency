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

void get_keys_test()
{
    DEBUG();

    generate_key();
    unsigned char shab_pbb[SHA256_DIGEST_LENGTH];
    unsigned char *buf_b_pbb[0x11F11] = {0};

    unsigned char shab_prkb[SHA256_DIGEST_LENGTH];
    unsigned char *buf_b_prkb[0x11F11] = {0};

    int pub_fd = open("./keys/rsa.pub", 0, 0x11F11);
    int priv_fd = open("./keys/rsa", 0, 0x11F11);
    safe_read(pub_fd, buf_b_prkb, 0x11F11);
    safe_read(priv_fd, buf_b_prkb, 0x11F11);
    SHA256(shab_pbb, strlen(buf_b_pbb), shab_pbb);
    SHA256(buf_b_prkb, strlen(buf_b_prkb), shab_prkb);
    close(pub_fd);
    close(priv_fd);

    get_keys();
    unsigned char shab_pbb1[SHA256_DIGEST_LENGTH];
    unsigned char *buf_b_pbb1[0x11F11] = {0};

    unsigned char shab_prkb1[SHA256_DIGEST_LENGTH];
    unsigned char *buf_b_prkb1[0x11F11] = {0};

    int pub_fd1 = open("./keys/rsa.pub", 0, 0x11F11);
    int priv_fd1 = open("./keys/rsa", 0, 0x11F11);
    safe_read(pub_fd1, buf_b_prkb1, 0x11F11);
    safe_read(priv_fd1, buf_b_prkb1, 0x11F11);
    SHA256(shab_pbb1, strlen(buf_b_pbb1), shab_pbb1);
    SHA256(buf_b_prkb1, strlen(buf_b_prkb1), shab_prkb1);
    close(pub_fd1);
    close(priv_fd1);

    if (strncmp(shab_pbb, shab_pbb1,SHA256_DIGEST_LENGTH) == 0 && strncmp(buf_b_prkb, buf_b_prkb1,SHA256_DIGEST_LENGTH) == 0)
    {
        TEST_PASSED("GET : Check private/public keys equality");
    }
    else
        TEST_FAILED("GET : Check private/public keys equality", "The keys before and after are not equal");
}