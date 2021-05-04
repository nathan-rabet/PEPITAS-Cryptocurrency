#ifndef MAIN_TEST_C
#define MAIN_TEST_C
#include "blockchain/wallet.h"

int main()
{
    get_keys();
    printf("%d",RSA_size(get_my_wallet()->pub_key));
    return 0;
}
#endif