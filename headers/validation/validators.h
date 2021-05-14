#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "cryptosystem/rsa.h"
#include "cryptosystem/hash.h"
#include "misc/files.h"
#include "misc/safe.h"

#define MAX_VALIDATORS_PER_BLOCK 512

/**
 * @brief Get the a comittee  RSA public keys on a specific epoch
 * @param block_height The height of the block you want a comitte from
 * @param nb_validators return value, the number of selected validators
 * @see The 'next block' is referring to block after 
 * the last block available OFFLINE
 * @return [*RSA]
 */
RSA **get_comittee(size_t block_height, int *nb_validators);

/**
 * @brief Get the a comittee  RSA public keys on a specific epoch
 * @param nb_validators return value, the number of selected validators
 * @see The 'next block' is referring to block after 
 * the last block available OFFLINE
 * @return [*RSA]
 */
RSA **get_next_comittee(int *nb_validators);

/**
 * @brief Get the total stake of the network 
 * (parse 'validators.state')
 * 
 * @return ssize_t 
 */
ssize_t get_validators_states_total_stake();

/**
 * @brief Get the number of validators of the network
 * (parse 'validators.state')
 * 
 * @return ssize_t 
 */
ssize_t get_validators_states_nb_validators();

/**
 * @brief Get the validators states block height validity
 * (parse 'validators.state')
 * 
 * @return ssize_t 
 */
ssize_t get_validators_states_block_height_validity();

/**
 * @brief Get a validator total stake
 * (parse 'validators.state')
 * 
 * @param validator_id The id of the validator in 'validators.state'
 * @return ssize_t 
 */
ssize_t get_validator_stake(size_t validator_id);

/**
 * @brief Get a validator power
 * (parse 'validators.state')
 * 
 * @param validator_id The id of the validator in 'validators.state'
 * @return ssize_t 
 */
ssize_t get_validator_power(size_t validator_id);

/**
 * @brief Get the validator pkey as RSA*
 * (parse 'validators.state')
 * 
 * @param validator_id The id of the validator in 'validators.state'
 * @return RSA* 
 */
RSA* get_validator_pkey(size_t validator_id);

/**
 * @brief Get the validator id in 'validators.state'
 * 
 * @param pkey The RSA public key
 * @return ssize_t, the validator index
 */
ssize_t get_validator_id(RSA* pkey);

/**
 * @brief Push an amount on the stake.
 * 
 * This will broadcast a stake push on the network.
 * 
 * @see The stake account public key is '1'
 * 
 * @param amount The amount to push
 * @return 0 if the broadcast succeeds, else returns -1
 */
int push_stake(size_t amount);

/**
 * @brief Pops an amount on the stake.
 * 
 * This will broadcast a stake pop on the network.
 * 
 * @see The stake account public key is '1'
 * 
 * @param amount The amount to pop
 * @return 0 if the broadcast succeeds, else returns -1
 */
int pop_stake(size_t amount);

#endif