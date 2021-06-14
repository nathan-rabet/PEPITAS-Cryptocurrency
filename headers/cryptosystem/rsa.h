#ifndef RSA_H
#define RSA_H

#include "blockchain/wallet.h"
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
#include <openssl/bn.h>
#include <openssl/crypto.h>
#include <string.h>

#define RSA_KEY_SIZE 366
#define RSA_FILE_TOTAL_SIZE 426
#define RSA_BEGIN_SIZE 31
#define RSA_END_SIZE 29

/**
 * @brief Get the keys object
 * 
 */
void get_keys(char *password);

/**
 * @brief Compare two RSA*
 * 
 */
static inline int cmp_public_keys(RSA *key1, RSA *key2){
    if (key1 && key2)
    {
        char my_pkey_string1[RSA_FILE_TOTAL_SIZE];
        char my_pkey_string2[RSA_FILE_TOTAL_SIZE];
        BIO *my_pub1 = BIO_new(BIO_s_mem());
        BIO *my_pub2 = BIO_new(BIO_s_mem());
        PEM_write_bio_RSAPublicKey(my_pub1, key1);
        PEM_write_bio_RSAPublicKey(my_pub2, key2);
        BIO_read(my_pub1, my_pkey_string1, RSA_FILE_TOTAL_SIZE);
        BIO_read(my_pub2, my_pkey_string2, RSA_FILE_TOTAL_SIZE);
        BIO_free(my_pub1);
        BIO_free(my_pub2);
        return strncmp(my_pkey_string1, my_pkey_string2, RSA_FILE_TOTAL_SIZE) == 0;
    }
    return false;
    // return key1 && key2 && 
    // !BN_cmp(RSA_get0_n(key1),RSA_get0_n(key2)) &&
    // !BN_cmp(RSA_get0_e(key1),RSA_get0_e(key2));
}

#endif