#include <assert.h>
#include <stdio.h>
#include "network/client.h"
#include "misc/safe.h"
#include "core/cryptosystem/coding.h"
#include "core/cryptosystem/rsa.h"
#include "core/blockchain/wallet.h"
#include <openssl/rsa.h>

int main()
{
    Wallet* w = get_my_wallet();
    generate_key();


    char* msg = "Coucou bande de nouilles!";
    return 0;
}