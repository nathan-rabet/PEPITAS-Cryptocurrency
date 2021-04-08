#include "tests_macros.h"
#include "cryptosystem/signature_test.h"
#include "cryptosystem/rsa_test.h"
#include "network/client_test.c"
#include "core/blockchain/block_test.h"
#include "core/validation/validations_test.c"

int main()
{
    DEBUG(get_keys_test);
    DEBUG(get_keys_equality_test);
    DEBUG(verify_sign_test);
    DEBUG(network_test);
    DEBUG(block_test);
    DEBUG(block_test);
    DEBUG(validations_test);

    return 0;
}