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

    Transaction create_account_transaction __attribute__((unused)) = {0};
    create_account_transaction.transaction_data->receiver_public_key = RSAPublicKey_dup(get_my_wallet()->pub_key);
    create_account_transaction.transaction_data->transaction_timestamp = time(NULL);

    return 0;
}