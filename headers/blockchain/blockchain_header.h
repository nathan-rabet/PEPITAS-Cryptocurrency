#ifndef BLOCKCHAIN_HEADER_H
#define BLOCKCHAIN_HEADER_H

#include "network/network.h"
#include "blockchain/block.h"
#include "cryptosystem/rsa.h"
#include "validation/validators.h"
#include <sys/stat.h>
#include <stdio.h>

void gen_blockchain_header(infos_st *infos);
size_t get_receiver_remaining_money(infos_st *infos, RSA *receiver_public_key);

#endif