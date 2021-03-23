#include <unistd.h>

#include "tests_macros.h"
#include "core/cryptosystem/coding.h"
#include "core/cryptosystem/rsa.h"
#include "core/blockchain/wallet.h"

void generate_key_test()
{
    DEBUG();
    generate_key();

    if (access(".keys/rsa.pub", F_OK) == 0 && access(".keys/rsa", F_OK) == 0)
    {
        TEST_PASSED("Generate private/public keys");
    }
    else
        TEST_FAILED("Generate private/public keys", "The files .keys/rsa and .keys/rsa.pub were not generated");
}