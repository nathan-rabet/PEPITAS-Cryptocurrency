#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <stdlib.h>
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include "cryptosystem/hash.h"
#include "cryptosystem/rsa.h"
#include "misc/files.h"
#include "misc/safe.h"
#include "misc/math.h"

struct validators_state_header
{
    size_t nb_validators;
    size_t total_stake;
    size_t block_height_validity;
};

struct validators_state_item
{
    char validator_pkey[RSA_KEY_SIZE];
    size_t user_stake;
    size_t validator_power;
};

#define MAX_VALIDATORS_PER_BLOCK 512

/**
 * @brief Init the `validators.state` file if it doesn't exists
 * 
 */
void init_validators_state();

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
 * @brief Check if the current user is a member of the next comitee
 * 
 * @return The id in the comittee, -1 if you are not member of the comittee
 */
int i_am_commitee_member();

/**
 * @brief Given a block, update the 'validators.state' with the transactions
 * 
 * @param block 
 * @return 0, -1 if the given block height is not 'validators.state' height + 1
 */
char update_validators_state(Block *block);

#endif