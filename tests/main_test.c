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
    get_keys_test();
    OpenSSL_add_all_algorithms();
    create_account();
    Wallet* wallet = get_my_wallet();
    char* msg = "Coucou bande de nouilles!";
    char* signature;
    size_t signature_len;
    sign_message(msg, &signature, &signature_len);
    printf("Sign len: %lu\n", signature_len);
    if (verify_sign(msg, signature, signature_len, wallet->pub_key))
    {
        printf("Signature is true!\n");
    }
    else
    {
        printf("Signature is false. :(\n");
    }
    
    generate_key_test();
    return 0;
}