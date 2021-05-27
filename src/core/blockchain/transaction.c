#include "blockchain/transaction.h"

void write_transactiondata(TransactionData *transaction, int fd)
{
    write(fd, &transaction->magic, sizeof(char));
    write(fd, &transaction->type, sizeof(char));

    BIO *pubkey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey, transaction->sender_public_key);
    int rsa_size = BIO_pending(pubkey);
    write(fd, &rsa_size, sizeof(int));
    char temp[1000];
    BIO_read(pubkey, temp, rsa_size);
    write(fd, temp, rsa_size);
    BIO_free(pubkey);

    BIO *pubkey2 = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey2, transaction->receiver_public_key);
    rsa_size = BIO_pending(pubkey2);
    write(fd, &rsa_size, sizeof(int));
    BIO_read(pubkey2, temp, rsa_size);
    write(fd, temp, rsa_size);
    BIO_free(pubkey2);

    write(fd, &transaction->amount, sizeof(size_t));
    write(fd, &transaction->receiver_remaining_money, sizeof(size_t));
    write(fd, &transaction->sender_remaining_money, sizeof(size_t));
    write(fd, &transaction->transaction_timestamp, sizeof(time_t));
    
    write(fd, transaction->cause, 512);
    write(fd, transaction->asset, 512);
}

void write_transaction(Transaction *transaction, int fd)
{
    write_transactiondata(&transaction->transaction_data, fd);
    write(fd, transaction->transaction_signature, 256);
}

void get_transaction_data(Transaction *trans, char **buff, size_t *index)
{
    *buff = realloc(*buff, *index + TRANSACTION_DATA_SIZE + 3000);


    memcpy(*buff + *index, &trans->transaction_data.magic, sizeof(char));
    *index += sizeof(char);
    memcpy(*buff + *index, &trans->transaction_data.type, sizeof(char));
    *index += sizeof(char);

    BIO *pubkey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey, trans->transaction_data.sender_public_key);
    int rsa_size = BIO_pending(pubkey);
    memcpy(*buff + *index, &rsa_size, sizeof(int));
    *index += sizeof(int);
    char temp[1000];
    BIO_read(pubkey, temp, rsa_size);
    memcpy(*buff + *index, temp, rsa_size);
    *index += rsa_size;

    BIO *pubkey2 = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey2, trans->transaction_data.receiver_public_key);
    rsa_size = BIO_pending(pubkey2);
    memcpy(*buff + *index, &rsa_size, sizeof(int));
    *index += sizeof(int);
    BIO_read(pubkey2, temp, rsa_size);
    memcpy(*buff + *index, temp, rsa_size);
    *index += rsa_size;
    BIO_free(pubkey);
    BIO_free(pubkey2);

    memcpy(*buff + *index, &trans->transaction_data.amount, sizeof(size_t));
    *index += sizeof(size_t);

    memcpy(*buff + *index, &trans->transaction_data.receiver_remaining_money, sizeof(size_t));
    *index += sizeof(size_t);

    memcpy(*buff + *index, &trans->transaction_data.sender_remaining_money, sizeof(size_t));
    *index += sizeof(size_t);

    memcpy(*buff + *index, &trans->transaction_data.transaction_timestamp, sizeof(time_t));
    *index += sizeof(time_t);
    memcpy(*buff + *index, trans->transaction_data.cause, 512);
    *index += 512;
    memcpy(*buff + *index, trans->transaction_data.asset, 512);
    *index += 512;
}

void convert_data_to_transactiondata(TransactionData *transactiondata, int fd)
{
    read(fd, &transactiondata->magic, sizeof(char));
    read(fd, &transactiondata->type, sizeof(char));
    int RSAsize;
    read(fd, &RSAsize, sizeof(int));

    char temp[1000];
    read(fd, temp, RSAsize);
    BIO *pubkey = BIO_new(BIO_s_mem());
    BIO_write(pubkey, temp, RSAsize);
    transactiondata->sender_public_key = PEM_read_bio_RSAPublicKey(pubkey, NULL, 0, NULL);
    BIO_free(pubkey);

    read(fd, &RSAsize, sizeof(int));
    read(fd, temp, RSAsize);
    BIO *pubkey1 = BIO_new(BIO_s_mem());
    BIO_write(pubkey1, temp, RSAsize);
    transactiondata->receiver_public_key = PEM_read_bio_RSAPublicKey(pubkey1, NULL, 0, NULL);
    BIO_free(pubkey1);

    read(fd, &transactiondata->amount, sizeof(size_t));
    read(fd, &transactiondata->receiver_remaining_money, sizeof(size_t));
    read(fd, &transactiondata->sender_remaining_money, sizeof(size_t));
    read(fd, &transactiondata->transaction_timestamp, sizeof(time_t));
    read(fd, transactiondata->cause, 512);
    read(fd, transactiondata->asset, 512);
}

void load_transaction(Transaction *transaction, int fd)
{
    if (transaction == NULL)
    {
        errx(EXIT_FAILURE, "Load_transaction: transction is not malloc");
    }
    convert_data_to_transactiondata(&transaction->transaction_data, fd);
    read(fd, transaction->transaction_signature, 256);
}

Transaction* load_pending_transaction(time_t timestamp) {
    char name[15] = {0};    
    sprintf(name, "pdt/%ld", timestamp);

    int transaction_file = open(name, O_RDONLY);
    if (transaction_file == -1)
        return NULL;
    Transaction *transaction = malloc(sizeof(Transaction));
    load_transaction(transaction, transaction_file);
    close(transaction_file);
    return transaction;
}

void add_pending_transaction(Transaction *transaction)
{
    char name[15] = {0};

    struct stat st;
    if (stat("pdt", &st) == -1)
    {
        mkdir("pdt", 0700);
    }

    sprintf(name, "pdt/%ld", transaction->transaction_data.transaction_timestamp);

    int pending_transaction_fd = open(name, O_CREAT | O_WRONLY, 0644);
    write_transaction(transaction,pending_transaction_fd);
    close(pending_transaction_fd);
}

Transaction create_new_transaction(infos_st *infos, char type, RSA* receiver_public_key, size_t amount, char cause[512], char asset[512]){
    Wallet *wallet = get_my_wallet();
    Transaction new_trans;
    TransactionData *data = &new_trans.transaction_data;
    data->magic = 1;
    data->type = type;
    switch (type)
    {
    case T_TYPE_DEFAULT:
    {
        data->sender_public_key = wallet->pub_key;
        data->receiver_public_key = receiver_public_key;
        data->amount = amount;
        data->sender_remaining_money = wallet->amount - amount;
        data->receiver_remaining_money = get_receiver_remaining_money(infos, receiver_public_key) + amount;
        break;
    }
    case T_TYPE_WITHDRAW_STAKE:
    {
        data->sender_public_key = NULL;
        data->receiver_public_key = wallet->pub_key;
        data->amount = amount;
        data->sender_remaining_money = wallet->stake_amount - amount;
        data->receiver_remaining_money = wallet->amount + amount;
            
        break;
    }
    case T_TYPE_ADD_STAKE:
    {
        data->sender_public_key = wallet->pub_key;
        data->receiver_public_key = NULL;
        data->amount = amount;
        data->sender_remaining_money = wallet->amount - amount;
        data->receiver_remaining_money = wallet->stake_amount + amount;

        break;
    }
    
    default:
        break;
    }
    data->transaction_timestamp = time(NULL);
    memcpy(data->cause, cause, 512);
    memcpy(data->asset, asset, 512);
    return new_trans;
}

void flush_pending_transactions(Transaction **transactions, size_t nb_transactions)
{
    for (size_t i = 0; i < nb_transactions; i++)
    {
        Transaction* trans = transactions[i];
        if (trans->transaction_data.type != T_TYPE_REWARD_STAKE && trans->transaction_data.type != T_TYPE_PUNISH_STAKE)
        {
            char temp[200];
            snprintf(temp, 200, "%li", trans->transaction_data.transaction_timestamp);
            remove(temp);
        }
    }
}