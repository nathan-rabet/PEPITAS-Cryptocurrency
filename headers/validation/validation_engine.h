#ifndef VALIDATIONS_H
#define VALIDATIONS_H

#include "blockchain/block.h"
#include "cryptosystem/signature.h"
#include "cryptosystem/rsa.h"
#include "cryptosystem/hash.h"
#include "network/get_data.h"
#include "misc/math.h"
#include "misc/files.h"
#include "misc/bits.h"
#include "misc/safe.h"
#include <string.h>
#include <math.h>
#include <openssl/bio.h>
#include <stdbool.h>
#include <openssl/evp.h>

#define VERIDCT_NO 0
#define VERIDCT_YES 1

extern connection *client_connections;

/**
 * @brief Broadcast a verdict about a block validity to the network
 * 
 * @param block The block awaiting validation
 * @param verdict The verdict : 0 -> "SHAME ! The block is not valid at all", 1 -> "The block is valid for me"
 * @return 0 if the broadcast suceed, -1 if not
 */
int send_verdict(Block *block, char verdict);

/**
 * @brief Validate some transactions
 * 
 * @see The verification must take into account :
 *  - Sender != receiver
 *  - If the sender signature is correct
 *  - If the sender exists in the blockchain and has enough money
 *  - If the receiver exists
 *  - If sender and receiver remaining money fields are correct
 * 
 * @param transaction_to_validate The transactions to validate
 * @param nb_transactions The number of transactions to validate
 * @param nb_returned_transactions The number of returned (valid) transactions
 * @return Transaction**, the valid transactions 
 */
Transaction ** validate_transactions(Transaction** transaction_to_validate,size_t nb_transactions,size_t * nb_returned_transactions);
int comital_validate_block(Block *block);
char plebe_verify_block(Block *block);

#endif