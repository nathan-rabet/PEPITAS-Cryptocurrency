#include <assert.h>
#include <stdio.h>
#include "network/client.h"
#include "misc/safe.h"
#include "core/cryptosystem/coding.h"
#include "core/cryptosystem/rsa.h"
#include "core/blockchain/wallet.h"
#include "cryptosystem/rsa_test.h"
#include <openssl/rsa.h>

int main()
{
    OpenSSL_add_all_algorithms();

    generate_key_test();
    get_keys_test();
    system("rm -rf ./.keys");
    get_generate_keys_equality_test();
    return 0;
}