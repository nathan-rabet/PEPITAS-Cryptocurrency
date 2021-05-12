#ifndef H_CLIENTM
#define H_CLIENTM

#include <signal.h>
#include <stdlib.h>
#include <string.h>

void new_transaction(char type, char *rc_pk, size_t amount, char cause[512], char asset[512]);

#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"
#include "misc/safe.h"
#include "blockchain/blockchain_header.h"
#include "blockchain/transaction.h"
#include <openssl/rsa.h>
#include "ui/ui.h"

#endif