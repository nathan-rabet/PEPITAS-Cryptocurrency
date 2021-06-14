#ifndef BLOCKCHAIN_HEADER_H
#define BLOCKCHAIN_HEADER_H

#include "network/network.h"
#include "blockchain/block.h"
#include "cryptosystem/rsa.h"
#include "validation/validators.h"
#include <sys/stat.h>
#include <stdio.h>

/**
 * @brief Generate block shared information
 * @deprecated
 * @param infos The information
 */
void gen_blockchain_header(infos_st *infos);

/**
 * @brief Get the receiver remaining money
 * 
 * @param infos Threads shared information
 * @param receiver_public_key The RSA public key of the receiver
 * @return size_t
 */
size_t get_receiver_remaining_money(infos_st *infos, RSA *receiver_public_key);

#endif