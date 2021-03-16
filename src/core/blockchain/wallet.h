#include <stdlib.h>
#include <stdbool.h>

typedef struct Wallet
{
    u_int64_t public_key;
    u_int64_t private_key;

    size_t amount;
    bool is_validator;
} Wallet;

/**
 * @brief Get my wallet object
 * 
 * @return Wallet 
 */
Wallet get_my_wallet();
