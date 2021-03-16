#include <stdlib.h>

/**
 * @brief Search how much money 'public_key' has in the blockchain
 * 
 * @param public_key The public key
 * @return ssize_t 
 */
ssize_t get_amount(u_int64_t public_key);