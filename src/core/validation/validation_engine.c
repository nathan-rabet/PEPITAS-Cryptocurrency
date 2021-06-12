#include "validation/validation_engine.h"

Transaction **validate_transactions(Transaction **transaction_to_validate, size_t nb_transactions, size_t *nb_returned_transactions)
{
    size_t nb_validated = 0;
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
        if (verify_transaction_signature(pending_transaction) == 0)
            continue;

        // ? bool time <= time NOW
        if (pending_transaction->transaction_data.transaction_timestamp > time(NULL))
            continue;

        // ? bool sender_not_equal_receiver
        if (cmp_public_keys(pending_transaction->transaction_data.sender_public_key, pending_transaction->transaction_data.receiver_public_key)) //
            continue;

        // Load the last chunk (to reverse-parse the blockchain)
        ChunkBlockchain *working_chunk = load_last_blockchain();
        size_t last_chunk_nb = working_chunk->chunk_nb;

        // Assertion to fulfilled
        bool sender_exists = 0;
        bool sender_enough_money = 0;
        bool sender_remaining_money_ok = 0;
        // bool receiver_exists = 0;

        switch (pending_transaction->transaction_data.type)
        {
        case T_TYPE_DEFAULT:
        {

            // Foreach block in the blockchain (reversed-way)
            for (int16_t b = working_chunk->nb_blocks-1; last_chunk_nb && (b >= 0 ||
                                                       (b = (working_chunk = load_blockchain(--last_chunk_nb)) != NULL ? working_chunk->nb_blocks-1 : 0));
                 b--)
            {
                // Foreach transations in a block (reversed-way)
                for (int32_t t = working_chunk->chunk[b]->block_data.nb_transactions-1; t >= 0; t--)
                {
                    Transaction *transaction = working_chunk->chunk[b]->block_data.transactions[t];

                    // ? bool sender_exists, first case (match with receiver_public_key)
                    if (!sender_exists &&
                        cmp_public_keys(pending_transaction->transaction_data.sender_public_key, transaction->transaction_data.receiver_public_key))
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
                             cmp_public_keys(pending_transaction->transaction_data.sender_public_key, transaction->transaction_data.sender_public_key))
                    {
                        sender_exists = 1;

                        // ? bool sender_remaining_money_ok
                        if (pending_transaction->transaction_data.sender_remaining_money == transaction->transaction_data.sender_remaining_money - pending_transaction->transaction_data.amount)
                            sender_remaining_money_ok = 1;

                        // ? bool sender_enough_money
                        if (transaction->transaction_data.sender_remaining_money >= pending_transaction->transaction_data.amount)
                            sender_enough_money = 1;
                    }

                    // // ? bool receiver_exists
                    // if (!receiver_exists && (cmp_public_keys(transaction->transaction_data.receiver_public_key, pending_transaction->transaction_data.receiver_public_key) || cmp_public_keys(transaction->transaction_data.sender_public_key, pending_transaction->transaction_data.receiver_public_key)))
                    //     receiver_exists = 1;
                }
            }
            // Add only if all the assertions are true
            if (sender_exists && sender_enough_money && sender_remaining_money_ok)
            {
                transactions[returned_transaction_i++] = transaction_to_validate[pending_t];
                nb_validated++;
            }
            break;
        }
        case T_TYPE_ADD_STAKE:
        {
            // Foreach block in the blockchain (reversed-way)
            for (int16_t b = working_chunk->nb_blocks-1; last_chunk_nb && (b >= 0 ||
                                                       (b = (working_chunk = load_blockchain(--last_chunk_nb)) != NULL ? working_chunk->nb_blocks-1 : 0));
                 b--)
            {
                // Foreach transations in a block (reversed-way)
                for (uint16_t t = 0; t < working_chunk->chunk[b]->block_data.nb_transactions; t++)
                {
                    Transaction *transaction = working_chunk->chunk[b]->block_data.transactions[t];

                    // ? bool sender_exists, first case (match with receiver_public_key)
                    if (!sender_exists &&
                        cmp_public_keys(pending_transaction->transaction_data.sender_public_key, transaction->transaction_data.receiver_public_key))
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
                             cmp_public_keys(pending_transaction->transaction_data.sender_public_key, transaction->transaction_data.sender_public_key))
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
            for (int16_t b = working_chunk->nb_blocks-1; last_chunk_nb && (b >= 0 ||
                                                       (b = (working_chunk = load_blockchain(--last_chunk_nb)) != NULL ? working_chunk->nb_blocks-1 : 0));
                 b--)
            {
                // Foreach transations in a block (reversed-way)
                for (uint16_t t = 0; !found && t < working_chunk->chunk[b]->block_data.nb_transactions; t++)
                {
                    Transaction *transaction = working_chunk->chunk[b]->block_data.transactions[t];

                    if (!sender_exists &&
                        cmp_public_keys(pending_transaction->transaction_data.sender_public_key, transaction->transaction_data.receiver_public_key))
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
    *nb_returned_transactions = nb_validated;
    return transactions;
}

char plebe_verify_block(Block *block)
{

    if (block == NULL || block->block_data.height == 0)
    {
        return -1;
    }

    // GET COMITTEE
    int nb_validators;
    RSA **validators = get_comittee(block->block_data.height-1, &nb_validators);

    // VERIFY EPOCH MAN IS IN THE VALIDATORS
    if (!cmp_public_keys(validators[block->block_data.epoch_id], block->block_data.validators_public_keys[block->block_data.epoch_id]))
    {
        for (int i = 0; i < nb_validators; i++)
        {
            RSA_free(validators[i]);
        }
        free(validators);
        return -1;
    }

    // VERIFY BLOCK SIGNATURE
    if (!verify_block_signature(*block))
    {
        for (int i = 0; i < nb_validators; i++)
        {
            RSA_free(validators[i]);
        }
        free(validators);
        return -1;
    }

    // FREE
    for (int i = 0; i < nb_validators; i++)
    {
        RSA_free(validators[i]);
    }
    free(validators);
    return 0;
}

int comital_validate_block(Block *block)
{
    Block *prev_block = get_block(block->block_data.height - 1);
    // TODO : TEST ALL BLOCK DATA VARIABLES (EXCEPT MAGIC)
    // ? prev_block->block_data.height
    if (prev_block->block_data.height != block->block_data.height - 1)
        return send_verdict(block, VERIDCT_NO);

    // ? block->block_data.block_timestamp
    if (block->block_data.block_timestamp > time(NULL))
        return send_verdict(block, VERIDCT_NO);

    // ? block->block_data.previous_block_hash
    size_t prev_block_data_size;
    char *data = get_blockdata_data(prev_block, &prev_block_data_size);
    char *prev_block_hash = sha384_data(data, prev_block_data_size);
    if (strncmp(prev_block_hash, block->block_data.previous_block_hash, SHA384_DIGEST_LENGTH * 2 + 1))
        return send_verdict(block, VERIDCT_NO);

    // ? block->block_data.validators_public_keys
    int nb_validators_local;
    RSA **next_comitee = get_next_comittee(&nb_validators_local);
    int nb_validators_in_new_block = 0;
    for (int i = 0; i < block->block_data.nb_validators; i++)
    {
        RSA *validator = block->block_data.validators_public_keys[i];
        nb_validators_in_new_block += validator != NULL;

        // ? [Comittee]== [block->block_data.validators_public_keys]
        if (!cmp_public_keys(validator, next_comitee[i]))
            return send_verdict(block, VERIDCT_NO);

        // // ? block->block_data.vote_signature[i]
        // if (!verify_signature(get_blockdata_data(block, &size), size, validator_signature, validator))
        //     return send_verdict(VERIDCT_NO);
    }
    // ? block->block_data.nb_validators
    if (nb_validators_local != nb_validators_in_new_block)
        return send_verdict(block, VERIDCT_NO);

    // ? block.block_data.epoch_id
    if (block->block_data.epoch_id >= block->block_data.nb_validators || block->block_data.epoch_id < 0)
        return send_verdict(block, VERIDCT_NO);

    // ? block.block_data.epoch_id and other stuff
    if (verify_block_signature(*block))
        return send_verdict(block, VERIDCT_NO);

    // ? block->block_data.transactions
    size_t nb_returned_transactions;
    if (validate_transactions(block->block_data.transactions, block->block_data.nb_transactions,
                              &nb_returned_transactions),
        nb_returned_transactions != block->block_data.nb_transactions)
        return send_verdict(block, VERIDCT_NO);

    // if (nb_nb_verdict_yes >= block->block_data.nb_validators - nb_nb_verdict_yes && !block->block_data.is_prev_block_valid)
    //     return send_verdict(VERIDCT_NO);
    // if (nb_nb_verdict_yes < block->block_data.nb_validators - nb_nb_verdict_yes && block->block_data.is_prev_block_valid)
    //     return send_verdict(VERIDCT_NO);

    return send_verdict(block, VERIDCT_YES);
}

int send_verdict(Block *block, char verdict)
{
    Wallet *wallet = get_my_wallet();

    int i = 0;
    int validator_index = 0;
    for (; i < block->block_data.nb_validators; i++)
    {
        RSA *validator = block->block_data.validators_public_keys[i];

        if (cmp_public_keys(validator, wallet->pub_key))
        {
            validator_index = i;
            break;
        }
    }

    if (i == block->block_data.nb_validators)
        return -1;

    size_t datalen;
    char *payload = create_vote_data(block, verdict, validator_index, &datalen);
    sign_message_with_key(payload, datalen, block->block_data.validators_public_keys[validator_index], payload + datalen);
    datalen += RSA_size(block->block_data.validators_public_keys[validator_index]) * 2;
    for (size_t i = 0; i < MAX_CONNECTION; i++)
    {
        if (client_connections[i].clientfd)
        {
            client_connections[i].demand = DD_SEND_VOTE;
            client_connections[i].Payloadsize = datalen;
            client_connections[i].Payload = malloc(datalen);
            memcpy(client_connections[i].Payload, payload, datalen);
            sem_post(&client_connections[i].lock);
        }
    }
    free(payload);
    return 0;
}
