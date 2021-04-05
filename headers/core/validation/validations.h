#include <stdlib.h>
#include <openssl/rsa.h>

/**
 * @brief Get the 'next block' validator RSA public key
 * @see The 'next block' is referring to block after 
 * the last block available OFFLINE
 * 
 * @param blockchain_flag The blockchain flag,
 * GENERAL_BLOCKCHAIN or VALIDATOR_BLOCKCHAIN
 * @return RSA*
 */
RSA* get_next_validator(char blockchain_flag);

/**
 * @brief Searches how much money 'public_key' has
 * 
 * @param public_key The RSA public key
 * @return The amount, or -1 in case of an error 
 */
ssize_t get_amount(RSA* public_key);