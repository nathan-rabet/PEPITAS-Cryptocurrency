#ifndef CODING_H
#define CODING_H

#include <stdlib.h>
#include <string.h>
#include <openssl/crypto.h>
#include <openssl/ssl3.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include "core/blockchain/wallet.h"

void sign_message(char *msg, u_int64_t len, char **signature);
void decrypt_msg(char *encoded_msg, u_int64_t len, char *msg);

#endif