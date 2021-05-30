#ifndef H_CLIENTM
#define H_CLIENTM

#include <signal.h>
#include <stdlib.h>
#include <string.h>

void new_transaction(char type, char *rc_pk, size_t amount, char cause[512], char asset[512]);

#include "network/network.h"
infos_st* get_infos();
void Validate();
void new_transaction(char type, char *rc_pk, size_t amount, char cause[512], char asset[512]);
void join_network_door(infos_st *infos);
void connection_to_others(infos_st *infos);
size_t update_blockchain_height(infos_st *infos);
void update_blockchain(infos_st *infos, size_t index_client);
void clear_transactions();
void update_pending_transactions_list(infos_st *infos);


#endif