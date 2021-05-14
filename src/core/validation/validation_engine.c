#include "validation/validation_engine.h"

// TODO deal with organisations
Transaction **validate_transactions(Transaction **transaction_to_validate, size_t nb_transactions, size_t *nb_returned_transactions)
{
    // Limits  the maximum amount of transactions
    *nb_returned_transactions = MIN(nb_transactions, MAX_TRANSACTIONS_PER_BLOCK);
    Transaction **transactions = malloc(*nb_returned_transactions * sizeof(Transaction *));

    // Save the state current_chunk
    size_t current_chunk_nb = load_blockchain(0)->chunk_nb;

    size_t returned_transaction_i = 0; // In case of bad transactions, we will not increment 'transaction_i'

    // Foreach transaction to validate
    for (size_t pending_t = 0; pending_t < *nb_returned_transactions; pending_t++)
    {
        Transaction *pending_transaction = transaction_to_validate[pending_t];

        // ? bool sender_signature_ok;
        if (!verify_transaction_signature(*transaction_to_validate[pending_t]))
            continue;

        // ? bool time <= time NOW
        if (pending_transaction->transaction_data.transaction_timestamp > time(NULL))
            continue;

        // ? bool sender_not_equal_receiver
        EVP_PKEY *pkey_sender = EVP_PKEY_new();
        EVP_PKEY *pkey_receiver = EVP_PKEY_new();
        EVP_PKEY_assign_RSA(pkey_sender, pending_transaction->transaction_data.sender_public_key);
        EVP_PKEY_assign_RSA(pkey_receiver, pending_transaction->transaction_data.receiver_public_key);
        if (EVP_PKEY_cmp(pkey_sender, pkey_receiver) != 0) //
            continue;
        EVP_PKEY_free(pkey_sender);
        EVP_PKEY_free(pkey_receiver);

        // Load the last chunk (to reverse-parse the blockchain)
        ChunkBlockchain *working_chunk = load_last_blockchain();
        size_t last_chunk_nb = working_chunk->chunk_nb;

        // Assertion to fulfilled
        bool sender_exists = 0;
        bool sender_enough_money = 0;
        bool sender_remaining_money_ok = 0;
        bool receiver_exists = 0;

        switch (pending_transaction->transaction_data.type)
        {
        case T_TYPE_DEFAULT:
        {

            // Foreach block in the blockchain (reversed-way)
            for (int16_t b = working_chunk->nb_blocks; b >= 0 ||
                                                       (b = (working_chunk = load_blockchain(--last_chunk_nb)) != NULL ? load_blockchain(0)->nb_blocks + 1 : 0) != 0;
                 b--)
            {
                // Foreach transations in a block (reversed-way)
                for (uint16_t t = 0; t < working_chunk->chunk[b]->block_data.nb_transactions; t++)
                {
                    Transaction *transaction = working_chunk->chunk[b]->block_data.transactions[t];

                    // ? bool sender_exists, first case (match with receiver_public_key)
                    if (!sender_exists &&
                        pending_transaction->transaction_data.sender_public_key == transaction->transaction_data.receiver_public_key)
                    {
                        sender_exists = 1;

                        // ? bool sender_remaining_money_ok
                        if (pending_transaction->transaction_data.sender_remaining_money == transaction->transaction_data.receiver_remaining_money - pending_transaction->transaction_data.amount)
                            sender_remaining_money_ok = 1;

                        // ? bool sender_enough_money
                        if (transaction->transaction_data.receiver_remaining_money >= pending_transaction->transaction_data.amount)
                            sender_enough_money = 1;
                    }

                    // ? bool sender_exists, second case (match with sender_public_key)
                    else if (!sender_exists &&
                             pending_transaction->transaction_data.sender_public_key == transaction->transaction_data.sender_public_key)
                    {
                        sender_exists = 1;

                        // ? bool sender_remaining_money_ok
                        if (pending_transaction->transaction_data.sender_remaining_money == transaction->transaction_data.sender_remaining_money - pending_transaction->transaction_data.amount)
                            sender_remaining_money_ok = 1;

                        // ? bool sender_enough_money
                        if (transaction->transaction_data.sender_remaining_money >= pending_transaction->transaction_data.amount)
                            sender_enough_money = 1;
                    }

                    // ? bool receiver_exists
                    if (!receiver_exists && (transaction->transaction_data.receiver_public_key == pending_transaction->transaction_data.receiver_public_key || transaction->transaction_data.sender_public_key == pending_transaction->transaction_data.receiver_public_key))
                        receiver_exists = 1;
                }
            }
            // Add only if all the assertions are true
            if (sender_exists && sender_enough_money && sender_remaining_money_ok && receiver_exists)
                transactions[returned_transaction_i++] = transaction_to_validate[pending_t];
            break;
        }
        case T_TYPE_ADD_STAKE:
        {
            // Foreach block in the blockchain (reversed-way)
            for (int16_t b = working_chunk->nb_blocks; b >= 0 ||
                                                       (b = (working_chunk = load_blockchain(--last_chunk_nb)) != NULL ? load_blockchain(0)->nb_blocks + 1 : 0) != 0;
                 b--)
            {
                // Foreach transations in a block (reversed-way)
                for (uint16_t t = 0; t < working_chunk->chunk[b]->block_data.nb_transactions; t++)
                {
                    Transaction *transaction = working_chunk->chunk[b]->block_data.transactions[t];

                    // ? bool sender_exists, first case (match with receiver_public_key)
                    if (!sender_exists &&
                        pending_transaction->transaction_data.sender_public_key == transaction->transaction_data.receiver_public_key)
                    {
                        sender_exists = 1;

                        // ? bool sender_remaining_money_ok
                        if (pending_transaction->transaction_data.sender_remaining_money == transaction->transaction_data.receiver_remaining_money - pending_transaction->transaction_data.amount)
                            sender_remaining_money_ok = 1;

                        // ? bool sender_enough_money
                        if (transaction->transaction_data.receiver_remaining_money >= pending_transaction->transaction_data.amount)
                            sender_enough_money = 1;
                    }

                    // ? bool sender_exists, second case (match with sender_public_key)
                    else if (!sender_exists &&
                             pending_transaction->transaction_data.sender_public_key == transaction->transaction_data.sender_public_key)
                    {
                        sender_exists = 1;

                        // ? bool sender_remaining_money_ok
                        if (pending_transaction->transaction_data.sender_remaining_money == transaction->transaction_data.sender_remaining_money - pending_transaction->transaction_data.amount)
                            sender_remaining_money_ok = 1;

                        // ? bool sender_enough_money
                        if (transaction->transaction_data.sender_remaining_money >= pending_transaction->transaction_data.amount)
                            sender_enough_money = 1;
                    }
                }
            }
            // Add only if all the assertions are true
            if (sender_exists && sender_enough_money && sender_remaining_money_ok)
                transactions[returned_transaction_i++] = transaction_to_validate[pending_t];
            break;
        }

        case T_TYPE_WITHDRAW_STAKE:
        {

            ssize_t validator_id = get_validator_id(pending_transaction->transaction_data.sender_public_key);

            // ? bool validator_exists
            if (validator_id == -1)
                continue;

            // ? bool stake_enough_money
            if (get_validator_stake(validator_id) < (ssize_t)pending_transaction->transaction_data.amount)
                continue;

            // Foreach block in the blockchain (reversed-way)
            char found = 0;
            for (int16_t b = working_chunk->nb_blocks;
                 !found && (b >= 0 ||
                            (b = (working_chunk = load_blockchain(--last_chunk_nb)) != NULL ? load_blockchain(0)->nb_blocks + 1 : 0) != 0);
                 b--)
            {
                // Foreach transations in a block (reversed-way)
                for (uint16_t t = 0; !found && t < working_chunk->chunk[b]->block_data.nb_transactions; t++)
                {
                    Transaction *transaction = working_chunk->chunk[b]->block_data.transactions[t];

                    if (!sender_exists &&
                        pending_transaction->transaction_data.sender_public_key == transaction->transaction_data.receiver_public_key)
                    {
                        // ? bool sender_remaining_money_ok
                        if (pending_transaction->transaction_data.sender_remaining_money == transaction->transaction_data.receiver_remaining_money - pending_transaction->transaction_data.amount)
                        {
                            transactions[returned_transaction_i++] = transaction_to_validate[pending_t];
                            found = 1;
                        }
                    }
                }
            }

            break;
        }
        default:
            return NULL;
        }
    }

    // Reset modified blockchain chunk
    load_blockchain(current_chunk_nb);

    return transactions;
}

char plebe_verify_block(Block *block)
{

    if (block == NULL || block->block_data.height == 0)
    {
        return 1;
    }

    // GET COMITTEE
    int nb_validators;
    RSA **validators = get_comittee(block->block_data.height, &nb_validators);

    // VERIFY EPOCH MAN IS IN THE VALIDATORS
    if (cmp_public_keys(validators[block->block_data.epoch_id], block->block_data.validators_public_keys[block->block_data.epoch_id]))
    {
        for (int i = 0; i < nb_validators; i++)
        {
            RSA_free(validators[i]);
        }
        free(validators);
        return 1;
    }

    // VERIFY BLOCK SIGNATURE
    if (verify_block_signature(*block) == 0)
    {
        for (int i = 0; i < nb_validators; i++)
        {
            RSA_free(validators[i]);
        }
        free(validators);
        return 1;
    }

    // FREE
    for (int i = 0; i < nb_validators; i++)
    {
        RSA_free(validators[i]);
    }
    free(validators);
    return 0;
}

int comital_validate_block(Block* block) {

    // TODO : TEST ALL BLOCK DATA VARIABLES (EXCEPT MAGIC)

    // TODO : TEST SIGNATURE

    // TODO : SEND VOTE

    return 0;
}