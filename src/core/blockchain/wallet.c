#include <time.h>

#include "core/blockchain/wallet.h"
#include "cryptosystem/rsa.h"
#include "core/blockchain/transaction.h"

Wallet* get_my_wallet() {

    static Wallet* my_account = NULL;

    if (my_account == NULL)
    {
        my_account = calloc(1, sizeof(Wallet));
    }

    return my_account;
}

int create_account() {
    
    get_keys();

    //Transaction create_account_transaction = {0};
    //create_account_transaction.receiver_public_key = RSAPublicKey_dup(wallet->keypair);
    //create_account_transaction.transaction_timestamp = time(NULL);

    return 0;
}