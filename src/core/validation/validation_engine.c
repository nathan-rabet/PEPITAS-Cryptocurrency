// #include "validation/validation_engine.h"

// // TODO
// Transaction **validate_transactions(Transaction *transaction_to_validate, size_t nb_transactions, size_t *nb_returned_transactions)
// {
//     // Get all pending transactions in 'pdt'
//     struct dirent **files = files_in_folder("pdt", *nb_returned_transactions);
//     if (files == -1)
//         return NULL;

//     // Limits  the maximum amount of transactions
//     *nb_returned_transactions = MIN(nb_returned_transactions, MAX_TRANSACTIONS_PER_BLOCK);
//     Transaction **transactions = malloc(*nb_returned_transactions * sizeof(Transaction *));

//     // Save the state current_chunk
//     size_t current_chunk_nb = load_blockchain(0)->chunk_nb;

//     int returned_transaction_i = 0; // In case of bad transactions, we will not increment 'transaction_i'

//     // Foreach transaction to validate
//     for (int pending_t = 0; pending_t < *nb_returned_transactions; pending_t++)
//     {

//         Transaction pending_transaction = transaction_to_validate[pending_t];

//         // Assertion to fulfilled
//         bool sender_exists = 0;
//         bool sender_enough_money = 0;
//         bool sender_remaining_money_ok = 0;
//         bool sender_enough_money = 0;
//         bool receiver_exists = 0;

//         // bool sender_signature_ok;
//         if (!verify_transaction_signature(transaction_to_validate[pending_t]))
//             continue;

//         // bool sender_not_receiver

//         // TODO : Sender != receiver
//         //if (pending_transaction.transaction_data->sender_public_key)

//         // Load the last chunk (to reverse-parse the blockchain)
//         ChunkBlockchain *working_chunk;
//         size_t last_chunk_nb = (working_chunk = load_last_blockchain())->chunk_nb;

//         // Load the transaction into the right place on the buffer
//         load_transaction(transactions[returned_transaction_i], files[pending_t]);

//         // Foreach block in the blockchain (reversed-way)
//         for (int16_t b = working_chunk->nb_blocks; b >= 0 || load_blockchain(--last_chunk_nb) != NULL, b = working_chunk->nb_blocks + 1; b--)
//         {
//             // Foreach transations in a block (reversed-way)
//             for (uint16_t t = 0; t < working_chunk->chunk[b]->block_data.nb_transactions; t++)
//             {
//                 Transaction *transaction = working_chunk->chunk[b]->block_data.transactions[t];

//                 if (!sender_exists && transaction->transaction_data->receiver_public_key == pending_transaction.transaction_data->sender_public_key) {
//                     sender_exists = 1;

//                     if (pending_transaction.transaction_data->sender_remaining_money == transaction->transaction_data.)

//                 }

//                 if (transaction->transaction_data->sender_public_key == pending_transaction.transaction_data->sender_public_key)

//                 // Check 'receiver_exists'
//                 if (!receiver_exists && transaction->transaction_data->receiver_public_key == pending_transaction.transaction_data->receiver_public_key || transaction->transaction_data->sender_public_key == pending_transaction.transaction_data->receiver_public_key)
//                     receiver_exists = 1;

                
//             }
//         }
//         // Increment only if all the assertions are true
//         returned_transaction_i += sender_exists && sender_enough_money && sender_remaining_money_ok && receiver_exists && sender_enough_money;
//     }
// }

// Block *create_next_block()
// {
// }