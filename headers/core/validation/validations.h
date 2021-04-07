#include <stdlib.h>
#include <openssl/rsa.h>

/**
 * @brief Get the 'next block' validators RSA public keys
 * @see The 'next block' is referring to block after 
 * the last block available OFFLINE
 * @return [*RSA]
 */
RSA** get_next_validators();

/**
 * @brief Searches how much money 'public_key' has
 * 
 * @param public_key The RSA public key
 * @return The amount, or -1 in case of an error 
 */
ssize_t get_amount(RSA* public_key);