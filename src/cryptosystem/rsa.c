#include "core/cryptosystem/rsa.h"
#include "core/blockchain/wallet.h"
#include <stdio.h>
#include <time.h>

// PUBLIC KEY : (n, e)
// PRIVATE KEY: (n, d)
void generate_key()
{
    RSA *keypair = RSA_generate_key(4096, 3, NULL, NULL);
    Wallet* wallet = get_my_wallet();
    wallet->keypair = keypair;
}