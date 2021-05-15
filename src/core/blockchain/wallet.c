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


void add_money_to_wallet(size_t money){
    Wallet* w = get_my_wallet();
    w->amount += money;
    char temp[40];
    snprintf(temp, 40, "%lu", w->amount);
    change_label_text(balance_1, temp);
    change_label_text(balance_2, temp);
}
void remove_money_from_wallet(size_t money){
    Wallet* w = get_my_wallet();
    w->amount -= money;
    char temp[40];
    snprintf(temp, 40, "%lu", w->amount);
    change_label_text(balance_1, temp);
    change_label_text(balance_2, temp);
}
void add_money_to_stake(size_t money){
    Wallet* w = get_my_wallet();
    w->amount += money;
    char temp[40];
    snprintf(temp, 40, "%lu", w->amount);
    change_label_text(stake_label1, temp);
    change_label_text(stake_label2, temp);
    change_label_text(stake_label3, temp);
}
void remove_money_from_stake(size_t money){
    Wallet* w = get_my_wallet();
    w->amount -= money;
    char temp[40];
    snprintf(temp, 40, "%lu", w->amount);
    change_label_text(stake_label1, temp);
    change_label_text(stake_label2, temp);
    change_label_text(stake_label3, temp);
}