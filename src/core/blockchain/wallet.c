#include <time.h>

#include "blockchain/wallet.h"
#include "cryptosystem/rsa.h"

Wallet* get_my_wallet() {

    static Wallet* my_account = NULL;

    if (my_account == NULL)
    {
        my_account = calloc(1, sizeof(Wallet));
    }

    return my_account;
}

int create_account() {
    
    get_keys(NULL);

    return 0;
}