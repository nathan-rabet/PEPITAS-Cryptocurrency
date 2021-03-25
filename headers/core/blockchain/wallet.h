#ifndef WALLET_H
#define WALLET_H

#include <openssl/rsa.h>
#include <stdlib.h>
#include <stdbool.h>
#include <openssl/rsa.h>

typedef struct Wallet
{
    RSA *keypair;

    size_t amount;
    bool is_validator;
} Wallet;

/**
 * @brief Get my wallet object
 * 
 * @return Wallet 
 */
Wallet* get_my_wallet();

#endif