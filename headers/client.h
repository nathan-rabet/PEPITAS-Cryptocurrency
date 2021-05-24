#ifndef H_CLIENTM
#define H_CLIENTM

#include <signal.h>
#include <stdlib.h>
#include <string.h>

void new_transaction(char type, char *rc_pk, size_t amount, char cause[512], char asset[512]);

#include "network/network.h"
infos_st* get_infos();

#endif