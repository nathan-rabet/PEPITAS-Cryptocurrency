#include "tests_macros.h"

#include "core/cryptosystem/coding.h"
#include "core/cryptosystem/rsa.h"
#include "core/blockchain/wallet.h"

void generate_key_test() {
    DEBUG();

    generate_key();
    Wallet* w = get_my_wallet();
    TEST_PASSED("Generate private/public keys");
}