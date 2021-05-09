#include "validation/validation_engine.h"

// TODO deal with organisations
Transaction **validate_transactions(Transaction *transaction_to_validate, size_t nb_transactions, size_t *nb_returned_transactions)
{
    // Limits  the maximum amount of transactions
    *nb_returned_transactions = MIN(nb_returned_transactions, MAX_TRANSACTIONS_PER_BLOCK);
    Transaction **transactions = malloc(*nb_returned_transactions * sizeof(Transaction *));

    // Save the state current_chunk
    size_t current_chunk_nb = load_blockchain(0)->chunk_nb;

    int returned_transaction_i = 0; // In case of bad transactions, we will not increment 'transaction_i'

    // Foreach transaction to validate
    for (int pending_t = 0; pending_t < *nb_returned_transactions; pending_t++)
    {
        Transaction pending_transaction = transaction_to_validate[pending_t];

        // Assertion to fulfilled
        bool sender_exists = 0;
        bool sender_enough_money = 0;
        bool sender_remaining_money_ok = 0;
        bool receiver_exists = 0;

        // ? bool sender_signature_ok;
        if (!verify_transaction_signature(transaction_to_validate[pending_t]))
            continue;

        // ? bool sender_not_equal_receiver
        EVP_PKEY *pkey_sender = EVP_PKEY_new();
        EVP_PKEY *pkey_receiver = EVP_PKEY_new();
        EVP_PKEY_assign_RSA(pkey_sender, pending_transaction.transaction_data.sender_public_key);
        EVP_PKEY_assign_RSA(pkey_receiver, pending_transaction.transaction_data.receiver_public_key);
        if (EVP_PKEY_cmp(pkey_sender, pkey_receiver) != 0) //
            continue;
        EVP_PKEY_free(pkey_sender);
        EVP_PKEY_free(pkey_receiver);

        // Load the last chunk (to reverse-parse the blockchain)
        ChunkBlockchain *working_chunk;
        size_t last_chunk_nb = (working_chunk = load_last_blockchain())->chunk_nb;

        // Load the transaction into the right place on the buffer
        load_transaction(transactions[returned_transaction_i], files[pending_t]);

        // Foreach block in the blockchain (reversed-way)
        for (int16_t b = working_chunk->nb_blocks; b >= 0 ||
                                                   (b = (working_chunk = load_blockchain(--last_chunk_nb)) != NULL ? load_blockchain(0)->chunk_nb + 1 : -1) != -1;
             b--)
        {
            // Foreach transations in a block (reversed-way)
            for (uint16_t t = 0; t < working_chunk->chunk[b]->block_data.nb_transactions; t++)
            {
                Transaction *transaction = working_chunk->chunk[b]->block_data.transactions[t];

                // ? bool sender_exists, first case (match with receiver_public_key)
                if (!sender_exists &&
                    pending_transaction.transaction_data.sender_public_key == transaction->transaction_data.receiver_public_key)
                {
                    sender_exists = 1;

                    // ? bool sender_remaining_money_ok
                    if (pending_transaction.transaction_data.sender_remaining_money == transaction->transaction_data.receiver_remaining_money - pending_transaction.transaction_data.amount)
                        sender_remaining_money_ok = 1;

                    // ? bool sender_enough_money
                    if (transaction->transaction_data.receiver_remaining_money >= pending_transaction.transaction_data.amount)
                        sender_enough_money = 1;
                }

                // ? bool sender_exists, second case (match with sender_public_key)
                else if (!sender_exists &&
                         pending_transaction.transaction_data.sender_public_key == transaction->transaction_data.sender_public_key)
                {
                    sender_exists = 1;

                    // ? bool sender_remaining_money_ok
                    if (pending_transaction.transaction_data.sender_remaining_money == transaction->transaction_data.sender_remaining_money - pending_transaction.transaction_data.amount)
                        sender_remaining_money_ok = 1;

                    // ? bool sender_enough_money
                    if (transaction->transaction_data.sender_remaining_money >= pending_transaction.transaction_data.amount)
                        sender_enough_money = 1;
                }

                // ? bool receiver_exists
                if (!receiver_exists && transaction->transaction_data.receiver_public_key == pending_transaction.transaction_data.receiver_public_key || transaction->transaction_data.sender_public_key == pending_transaction.transaction_data.receiver_public_key)
                    receiver_exists = 1;
            }
        }
        // Add only if all the assertions are true
        if (sender_exists && sender_enough_money && sender_remaining_money_ok && receiver_exists)
            transactions[returned_transaction_i++] = &transaction_to_validate[pending_t];
    }

    // Reset modified blockchain chunk
    load_blockchain(current_chunk_nb);
}

Block *create_next_block()
{
}