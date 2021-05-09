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

int cmp_public_keys(RSA *key1, RSA *key2);

#endif