#include "wallet.h"

Wallet get_my_wallet() {

    static Wallet my_account = {0};

    return my_account;
}
