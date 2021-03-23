#include <stdlib.h>
#include <stdbool.h>
#include <openssl/rsa.h>

typedef struct Wallet
{
    RSA * public_key;
    RSA * private_key;

    size_t amount;
    bool is_validator;
} Wallet;

/**
 * @brief Get my wallet object
 * 
 * @return Wallet 
 */
Wallet get_my_wallet();

/**
 * @brief Create a account,
 * broadcast public credentials to the network
 * 
 * @return 0 if the broadcast succeed, 1 if not
 */
int create_account();
