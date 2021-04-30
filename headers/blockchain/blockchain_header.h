#ifndef BLOCKCHAIN_HEADER_H
#define BLOCKCHAIN_HEADER_H

#include "blockchain/block.h"
#include "network/network.h"
#include <sys/stat.h>
#include <stdio.h>

void gen_blockchain_header(infos_st *infos);

#endif