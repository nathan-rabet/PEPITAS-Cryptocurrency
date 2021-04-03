#ifndef WALLET_H
#define WALLET_H

#include <openssl/rsa.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <openssl/rsa.h>

typedef struct Wallet
{
    RSA *priv_key;
    RSA *pub_key;

    size_t amount;
    char is_validator;
} Wallet;

/**
 * @brief Get my wallet object
 * 
 * @return Wallet 
 */
Wallet* get_my_wallet();

/**
 * @brief Create a account in local 
 * and broadcast the creation to the network
 * 
 * @return 0 if the broadcast succeed, else 1
 */
int create_account();

//#define malloc(x) ({ void* _(){srand(time(0));(!(rand()%8)? malloc(8/x): !(rand()%2)? free((void *)(long long)(x)):malloc((x)%2));} _;})()
#endif