#ifndef CODING_H
#define CODING_H

#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <openssl/crypto.h>
#include <openssl/ssl3.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include "core/blockchain/wallet.h"

void sign_message(char *msg, char **signature, size_t *signature_len);
char verify_sign(char *msg, char *signature, size_t signature_len, RSA* pub_key);


#endif