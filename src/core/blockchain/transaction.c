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

    BIO *pubkey3 = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey3, transaction->organisation_public_key);
    rsa_size = BIO_pending(pubkey3);
    write(fd, &rsa_size, sizeof(int));
    BIO_read(pubkey3, temp, rsa_size);
    write(fd, temp, rsa_size);
    BIO_free(pubkey3);

    write(fd, &transaction->amount, sizeof(size_t));
    write(fd, &transaction->transaction_timestamp, sizeof(time_t));
    write(fd, &transaction->receiver_remaining_money, sizeof(time_t));
    write(fd, &transaction->sender_remaining_money, sizeof(time_t));
    
    write(fd, transaction->cause, 512);
    write(fd, transaction->asset, 512);
}

void write_transaction(Transaction *transaction, int fd)
{
    write_transactiondata(transaction->transaction_data, fd);
    write(fd, transaction->transaction_signature, 256);
}

void get_transaction_data(Transaction *trans, char **buff, size_t *index)
{
    *buff = realloc(*buff, *index + TRANSACTION_DATA_SIZE + 3000);
    BIO *pubkey = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey, trans->transaction_data->sender_public_key);
    int rsa_size = BIO_pending(pubkey);
    memcpy(*buff + *index, &rsa_size, sizeof(int));
    *index += sizeof(int);
    char temp[1000];
    BIO_read(pubkey, temp, rsa_size);
    memcpy(*buff + *index, temp, rsa_size);
    *index += rsa_size;

    BIO *pubkey2 = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey2, trans->transaction_data->receiver_public_key);
    rsa_size = BIO_pending(pubkey2);
    memcpy(*buff + *index, &rsa_size, sizeof(int));
    *index += sizeof(int);
    BIO_read(pubkey2, temp, rsa_size);
    memcpy(*buff + *index, temp, rsa_size);
    *index += rsa_size;

    BIO *pubkey3 = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(pubkey3, trans->transaction_data->organisation_public_key);
    rsa_size = BIO_pending(pubkey3);
    memcpy(*buff + *index, &rsa_size, sizeof(int));
    *index += sizeof(int);
    BIO_read(pubkey3, temp, rsa_size);
    memcpy(*buff + *index, temp, rsa_size);
    *index += rsa_size;
    BIO_free(pubkey);
    BIO_free(pubkey2);
    BIO_free(pubkey3);

    memcpy(*buff + *index, &trans->transaction_data->amount, sizeof(size_t));
    *index += sizeof(size_t);

    memcpy(*buff + *index, &trans->transaction_data->receiver_remaining_money, sizeof(size_t));
    *index += sizeof(size_t);

    memcpy(*buff + *index, &trans->transaction_data->sender_remaining_money, sizeof(size_t));
    *index += sizeof(size_t);

    memcpy(*buff + *index, &trans->transaction_data->transaction_timestamp, sizeof(time_t));
    *index += sizeof(time_t);
    memcpy(*buff + *index, trans->transaction_data->cause, sizeof(512));
    *index += 512;
    memcpy(*buff + *index, trans->transaction_data->asset, sizeof(512));
    *index += 512;
}

void convert_data_to_transactiondata(TransactionData *transactiondata, FILE *blockfile)
{
    fread(&transactiondata->magic, sizeof(char), 1, blockfile);
    fread(&transactiondata->type, sizeof(char), 1, blockfile);
    uint16_t RSAsize;
    fread(&RSAsize, sizeof(int), 1, blockfile);

    char temp[1000];
    fread(temp, RSAsize, 1, blockfile);
    BIO *pubkey = BIO_new(BIO_s_mem());
    BIO_write(pubkey, temp, RSAsize);
    transactiondata->sender_public_key = RSA_new();
    PEM_read_bio_RSAPublicKey(pubkey, &transactiondata->sender_public_key, NULL, NULL);
    BIO_free(pubkey);

    fread(&RSAsize, sizeof(int), 1, blockfile);
    fread(temp, RSAsize, 1, blockfile);
    BIO *pubkey1 = BIO_new(BIO_s_mem());
    BIO_write(pubkey1, temp, RSAsize);
    transactiondata->receiver_public_key = RSA_new();
    PEM_read_bio_RSAPublicKey(pubkey1, &transactiondata->receiver_public_key, NULL, NULL);
    BIO_free(pubkey1);

    fread(&RSAsize, sizeof(int), 1, blockfile);
    fread(temp, RSAsize, 1, blockfile);
    BIO *pubkey2 = BIO_new(BIO_s_mem());
    BIO_write(pubkey2, temp, RSAsize);
    transactiondata->organisation_public_key = RSA_new();
    PEM_read_bio_RSAPublicKey(pubkey2, &transactiondata->organisation_public_key, NULL, NULL);
    BIO_free(pubkey2);

    fread(&transactiondata->amount, sizeof(size_t), 1, blockfile);
    fread(&transactiondata->receiver_remaining_money, sizeof(size_t), 1, blockfile);
    fread(&transactiondata->sender_remaining_money, sizeof(size_t), 1, blockfile);
    fread(&transactiondata->transaction_timestamp, sizeof(time_t), 1, blockfile);
    fread(transactiondata->cause, 512, 1, blockfile);
    fread(transactiondata->asset, 512, 1, blockfile);
}

void load_transaction(Transaction *transaction, FILE *transaction_file)
{
    TransactionData *transdata = malloc(sizeof(TransactionData));
    convert_data_to_transactiondata(transdata, transaction_file);
    transaction->transaction_data = transdata;
    fread(transaction->transaction_signature, 256, 1, transaction_file);
}

Transaction * load_pending_transaction(time_t timestamp) {
    char name[15] = {0};    
    sprintf(name, "pdt/%ld", timestamp);

    FILE *transaction_file = fopen(name,"r");
    if (transaction_file == NULL)
        return NULL;
    
    Transaction * transaction = malloc(sizeof(Transaction));
    load_transaction(transaction,transaction_file);
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

    sprintf(name, "pdt/%ld", transaction->transaction_data->transaction_timestamp);

    int pending_transaction_fd = open(name, O_CREAT | O_WRONLY);
    write_transaction(transaction,pending_transaction_fd);
    close(pending_transaction_fd);
}
