#ifndef WALLET_H
#define WALLET_H

#include <openssl/rsa.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "ui/labels.h"

typedef struct Wallet
{
    RSA *priv_key; // My private key
    RSA *pub_key; // My public key

    size_t amount; // My PEPITAS amount
    size_t stake_amount; // My stake amount
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

/**
 * @brief Add money to my wallet
 * 
 * @param money The amount of PEPITAS
 */
void add_money_to_wallet(size_t money);

/**
 * @brief Remove money from my wallet
 * 
 * @param money The amount of PEPITAS
 */
void remove_money_from_wallet(size_t money);

/**
 * @brief Add money to my stake
 * 
 * @param money The amount of PEPITAS
 */
void add_money_to_stake(size_t money);

/**
 * @brief Withdraw money from my stake
 * 
 * @param money The amount of PEPITAS
 */
void remove_money_from_stake(size_t money);

#endif