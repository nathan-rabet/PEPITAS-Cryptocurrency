#ifndef WALLET_H
#define WALLET_H

#include <openssl/rsa.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <openssl/rsa.h>
#include "ui/labels.h"

typedef struct Wallet
{
    RSA *priv_key;
    RSA *pub_key;

    size_t amount;
    size_t stake_amount;
    char is_validator;
} Wallet;

/**
 * @brief Get my wallet object
 * 
 * @return Wallet 
 */
Wallet* get_my_wallet();

/**
 * @brief Creates an account in local 
 * and broadcasts the creation to the network
 * 
 * @return 0 if the broadcast succeeds, otherwise 1
 */
int create_account();

void add_money_to_wallet(size_t money);
void remove_money_from_wallet(size_t money);
void add_money_to_stake(size_t money);
void remove_money_from_stake(size_t money);

#endif