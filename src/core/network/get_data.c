#include "network/get_data.h"

size_t process_header(char *header, int sockfd, infos_st *infos)
{
    if (strncmp(HD_GET_CLIENT_LIST, header, strlen(HD_GET_CLIENT_LIST)) == 0)
    {
        SERVERMSG
        printf("Recived header HD_GET_CLIENT_LIST\n");
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);

        if (getpeername(sockfd, (struct sockaddr *restrict)&client_addr, &client_addr_len) == -1)
            err(EXIT_FAILURE, "Failed to recover client IP address\n");

        char ip_str[39];
        if (inet_ntop(AF_INET, &(client_addr.sin_addr), ip_str, INET_ADDRSTRLEN) == NULL)
            err(EXIT_FAILURE, "Failed to convert client IP address to string\n");
        return send_client_list(IM_SERVER, sockfd, ip_str);
    }
    if (strncmp(HD_CONNECTION_TO_NETWORK, header, strlen(HD_CONNECTION_TO_NETWORK)) == 0)
    {
        SERVERMSG
        printf("Recived header HD_CONNECTION_TO_NETWORK\n");
        return 1;
    }
    if (strncmp(HD_CONNECTION_TO_NODE, header, strlen(HD_CONNECTION_TO_NODE)) == 0)
    {
        SERVERMSG
        printf("Recived header HD_CONNECTION_TO_NODE\n");
        return 1;
    }
    if (strncmp(HD_SEND_CLIENT_LIST, header, 8) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_CLIENT_LIST\n");
        return fetch_client_list(IM_CLIENT, sockfd);
    }
    if (strncmp(HD_GET_BLOCKS, header, strlen(HD_GET_BLOCKS)) == 0)
    {
        SERVERMSG
        printf("Recived header HD_GET_BLOCKS\n");
        return read_get_blocks(sockfd, infos);
    }
    if (strncmp(HD_ACTUAL_HEIGHT, header, strlen(HD_ACTUAL_HEIGHT)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_ACTUAL_HEIGHT\n");
        return read_actual_height(sockfd);
    }
    if (strncmp(HD_SEND_BLOCK, header, strlen(HD_SEND_BLOCK)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_BLOCK\n");
        read_send_block(sockfd);
        return 0;
    }
    if (strncmp(HD_GET_PENDING_TRANSACTION_LIST, header, strlen(HD_GET_PENDING_TRANSACTION_LIST)) == 0)
    {
        SERVERMSG
        printf("Recived header HD_GET_PENDING_TRANSACTION_LIST\n");
        send_pending_transaction_list(sockfd);
        return 0;
    }
    if (strncmp(HD_SEND_PENDING_TRANSACTION_LIST, header, strlen(HD_SEND_PENDING_TRANSACTION_LIST)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_PENDING_TRANSACTION_LIST\n");
        read_send_pending_transaction_list(sockfd, infos);
        return 0;
    }
    if (strncmp(HD_REJECT_DEMAND, header, strlen(HD_REJECT_DEMAND)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_REJECT_DEMAND\n");
        return 0;
    }
    if (strncmp(HD_SEND_PENDING_TRANSACTION, header, strlen(HD_SEND_PENDING_TRANSACTION)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_PENDING_TRANSACTION\n");
        return read_send_pending_transaction(sockfd, infos);
    }
    if (strncmp(HD_GET_PENDING_TRANSACTION, header, strlen(HD_GET_PENDING_TRANSACTION)) == 0)
    {
        SERVERMSG
        printf("Recived header HD_GET_PENDING_TRANSACTION\n");
        return read_get_pending_transaction(sockfd);
    }
    if (strncmp(HD_SEND_EPOCH_BLOCK, header, strlen(HD_SEND_EPOCH_BLOCK)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_EPOCH_BLOCK\n");
        return read_epoch_block(sockfd);
    }
    if (strncmp(HD_SEND_VOTE, header, strlen(HD_SEND_VOTE)) == 0)
    {
        CLIENTMSG
        printf("Recived header HD_SEND_EPOCH_BLOCK\n");
        return read_vote(sockfd, infos);
    }

    // WARNINGMSG("Can't read header!")

    return 0;
}

int fetch_client_list(char who, int fd)
{
    int index = 0;
    int family = 0;
    char *hostname = malloc(SIZE_OF_HOSTNAME);
    int nb_ng;
    read(fd, &nb_ng, sizeof(int));

    while (index < nb_ng)
    {
        read(fd, &family, sizeof(int));

        int hostname_size;
        // IPv4
        if (family == AF_INET)
            hostname_size = 15;
        // IPv6
        else
            hostname_size = 39;

        read(fd, hostname, hostname_size);
        index++;

        set_neighbour(who, hostname, family);
    }
    free(hostname);
    return 0;
}

size_t read_header(int sockfd, infos_st *infos)
{
    // Waiting header for server and read it
    char *buffer;
    size_t buffer_size;
    ssize_t nb_read;

    // Get Header
    nb_read = safe_read(sockfd, (void *)&buffer, &buffer_size);

    if (nb_read != -1)
    {
        size_t r = process_header(buffer, sockfd, infos);
        free(buffer);
        return r;
    }
    return 0;
}

int read_get_blocks(int fd, infos_st *infos)
{
    get_blocks_t demand;
    ssize_t r;
    if ((r = recv(fd, &demand, sizeof(get_blocks_t), MSG_WAITALL)) != sizeof(get_blocks_t))
    {
        SERVERMSG
        printf("OOF Failed to load payload: %lx\n", r);
    }
    for (char i = 0; i < demand.nb_demands; i++)
    {
        size_t height = *(demand.blocks_height + i);
        if (height == 0)
        {
            send_actual_height(fd, infos);
        }
        else
        {
            if (height <= infos->actual_height)
            {
                send_send_block(fd, height);
            }
            else
            {
                send_reject_demand(fd);
            }
        }
    }
    return 0;
}

size_t read_actual_height(int fd)
{
    size_t ac;
    read(fd, &ac, sizeof(size_t));
    return ac;
}

int read_send_block(int fd)
{
    int blockfile;
    char dir[256];
    char temp[1024];
    size_t block_height;
    size_t bc_size;
    ssize_t r = read(fd, &block_height, sizeof(size_t));
    if (r != sizeof(size_t))
        return -1;
    r = read(fd, &bc_size, sizeof(size_t));
    if (r != sizeof(size_t))
        return -1;
    snprintf(dir, 256, "data/blockchain/c%iblock%lu", fd, block_height);

    blockfile = open(dir, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (blockfile == -1)
    {
        CLIENTMSG
        printf("Can't create c%iblock%lu file\n", fd, block_height);
        return -1;
    }

    while ((r = read(fd, temp, bc_size > 1024 ? 1024 : bc_size)) && bc_size > 0)
    {
        if (r == -1)
            errx(EXIT_FAILURE, "Can't read block %lu in connection fd: %i", block_height, fd);
        safe_write(blockfile, temp, r);
        bc_size -= r;
    }
    if (bc_size > 0)
    {
        WARNINGMSG("Failed to read all the block!\n")
    }

    CLIENTMSG
    printf("Recived block %lu in connection fd: %i\n", block_height, fd);

    // LOAD BLOCK
    Block *block = malloc(sizeof(Block));
    lseek(blockfile, 0L, SEEK_SET);
    convert_data_to_block(block, blockfile);
    close(blockfile);

    // ADD TO BLOCKCHAIN IF TRUE
    if (plebe_verify_block(block) == 0)
    {
        CLIENTMSG
        printf("The block %lu is valid!\n", block_height);
        snprintf(temp, 256, "data/blockchain/block%lu", block_height);
        int ret = rename(dir, temp);
        update_wallet_with_block(*block);
        get_infos()->actual_height++;

        if (ret == 0)
        {
            CLIENTMSG
            printf("File renamed successfully\n");
        }
        else
        {
            CLIENTMSG
            printf("Error: unable to rename the file\n");
        }
    }
    else
    {
        CLIENTMSG
        printf("The block %lu is not valid.\n", block_height);
        int ret = remove(dir);
        if (ret == 0)
        {
            CLIENTMSG
            printf("File remove successfully\n");
        }
        else
        {
            CLIENTMSG
            printf("Error: unable to remove the file\n");
        }
    }

    free_block(block);
    return 0;
}

int read_vote(int fd, infos_st* infos)
{

    // RECUP THE VOTE DATA

    size_t RSAsize = 0;
    size_t height = 0;
    int epoch_id = 0;
    char vote = 0;
    char validator_signature[2 * SHA384_DIGEST_LENGTH];
    char data[900];
    size_t data_length = sizeof(size_t);
    read(fd, data, sizeof(size_t));
    RSAsize = *(size_t *)(data);

    BIO *pubkey = BIO_new(BIO_s_mem());
    read(fd, (data + data_length), RSAsize);
    BIO_write(pubkey, (data + data_length), RSAsize);
    RSA *rsa_validator_key;
    rsa_validator_key = PEM_read_bio_RSAPublicKey(pubkey, NULL, 0, NULL);
    data_length += RSAsize;

    read(fd, (data + data_length), sizeof(size_t));
    height = *(size_t *)(data + data_length);
    data_length += sizeof(size_t);

    read(fd, (data + data_length), sizeof(int));
    epoch_id = *(int *)(data + data_length);
    data_length += sizeof(int);

    read(fd, (data + data_length), sizeof(char));
    vote = *(char *)(data + data_length);
    data_length += sizeof(char);

    // VERIFY SIGNATURE TODO
    read(fd, validator_signature, SIGNATURE_LEN);
    // if (!verify_signature(data, data_length, validator_signature, rsa_validator_key))
    // {
    //     RSA_free(rsa_validator_key);
    //     SERVERMSG
    //     printf("Vote is not valid.\n");
    //     return -1;
    // }

    SERVERMSG
    printf("Vote is valid!\n");

    // SEARCH VALIDATOR
    Block *block = get_epoch(epoch_id, infos->actual_height+1);

    if (block == NULL)
    {
        SERVERMSG
        printf("Epoch for the vote does not exist.\n");
        RSA_free(rsa_validator_key);
        return -1;
    }

    if (block->block_data.height != height)
    {
        SERVERMSG
        printf("Vote height is not valid.\n");
        RSA_free(rsa_validator_key);
        return -1;
    }

    int validator_index = 0;
    for (; validator_index < block->block_data.nb_validators; validator_index++)
    {
        if (cmp_public_keys(block->block_data.validators_public_keys[validator_index], rsa_validator_key))
        {
            break;
        }
    }

    if (validator_index == block->block_data.nb_validators)
    {
        SERVERMSG
        printf("Validator key is not in the epoch.\n");
        RSA_free(rsa_validator_key);
        return -1;
    }
    if (validator_index == block->block_data.nb_validators - 1)
    {
        validator_index = epoch_id;
    }

    // APPLY VOTE
    memcpy(block->vote_signature[validator_index], validator_signature, 2 * SHA384_DIGEST_LENGTH);
    if (vote == 1)
    {
        block->validators_votes[validator_index / 8] |= 1 << (validator_index % 8);
    }
    else
    {
        block->validators_votes[validator_index / 8] &= ~(1 << (validator_index % 8));
    }

    // CHECK VOTE NUMBERS
    char all_vote = true;
    for (int i = 0; i <= block->block_data.nb_validators/8; i++)
    {
        if (~(block->validators_votes[i] | ((i == (block->block_data.nb_validators/8) ? ~0 : 0) << (block->block_data.nb_validators%8))))
        {
            all_vote = false;
        }
    }

    // ADD BLOCK TO THE BLOCKCHAIN
    if (all_vote) {
        char dir_src[300];
        char dir_dest[300];
        snprintf(dir_src, 300, "data/epoch/epoch%luid%d", block->block_data.height, block->block_data.epoch_id);
        snprintf(dir_dest, 300, "data/blockchain/block%lu", block->block_data.height);
        move_file(dir_src, dir_dest);
        remove(dir_src);

        SERVERMSG
        printf("Block %lu added to the blockchain!\n", block->block_data.height);

        infos_st *infos = get_infos();
        infos->actual_height++;
        infos->as_epoch = 0;
        infos->validator_id = i_am_commitee_member();
        if (infos->validator_id < 0) {

            MANAGERMSG
            printf("You are not in the next comitte.\n");
        }
        else
        {
            MANAGERMSG
            printf("You are in the next comitte.\n");
        }
        update_wallet_with_block(*block);
    }
    RSA_free(rsa_validator_key);
    free_block(block);
    return 0;
}

int read_epoch_block(int fd)
{
    int id = 0;
    size_t height = 0;
    size_t bc_size = 0;
    char dir[256];
    char temp[1024];
    ssize_t r;
    read(fd, &id, sizeof(int));
    read(fd, &height, sizeof(size_t));
    read(fd, &bc_size, sizeof(size_t));


    // CHECK LOWER ID EXISTING
    for (int i = 0; i < id; i++)
    {
        snprintf(dir, 256, "data/epoch/epoch%luid%d", height, i);
        if (!access("data/keys/rsa.pub", F_OK)) {

            CLIENTMSG
            printf("Reject epoch because epoch id %d exist!\n", i);
            // CLEAR FD
            while ((r = read(fd, temp, bc_size > 1024 ? 1024 : bc_size)) && bc_size > 0)
            {
                if (r == -1)
                    errx(EXIT_FAILURE, "Can't read epoch %lu in connection fd: %i", height, fd);
                bc_size -= r;
            }
            return -1;
        }
    }
    
    snprintf(dir, 256, "data/epoch/epoch%luid%d", height, id);

    // CREATE EPOCH FILE
    int blockfile = open(dir, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (blockfile == -1)
    {
        CLIENTMSG
        printf("Can't create c%iepoch%lu file\n", fd, height);
        return -1;
    }

    // READ EPOCH
    while ((r = read(fd, temp, bc_size > 1024 ? 1024 : bc_size)) && bc_size > 0)
    {
        if (r == -1)
            errx(EXIT_FAILURE, "Can't read epoch %lu in connection fd: %i", height, fd);
        safe_write(blockfile, temp, r);
        bc_size -= r;
    }
    if (bc_size > 0)
    {
        WARNINGMSG("Failed to read all the epoch!\n")
    }

    CLIENTMSG
    printf("Recived epoch %lu in connection fd: %i\n", height, fd);

    return epoch_validation_process(blockfile, height, id);
}

int epoch_validation_process(int blockfile, size_t height, int id) {
    infos_st *infos = get_infos();

    // IS NEXT BLOCK
    if (infos->actual_height + 1 == height)
    {
        MANAGERMSG
        printf("Epoch is the next!\n");

        // LOAD BLOCK
        Block *epoch = malloc(sizeof(Block));
        lseek(blockfile, 0L, SEEK_SET);
        convert_data_to_block(epoch, blockfile);
        close(blockfile);

        // VOTE FOR THIS EPOCH
        if (infos->validator_id >= 0)
        {
            int v = comital_validate_block(epoch);
            MANAGERMSG
            printf("Vote %d for the epoch %d!\n", v, id);
        }

        // CREATE NEW EPOCH
        if (infos->is_validator == 1 && !infos->as_epoch && infos->pdt > 0)
        {
            Validate();
        }
        free_block(epoch);
        // START TIMER TODO
        return 0;
    }


    // ADD NEXT BLOCK TO THE BLOCKCHAIN
    if (infos->actual_height + 2 == height)
    {
        for (size_t i = 0; i < MAX_VALIDATORS_PER_BLOCK; i++)
        {
            Block *my_new_epoch = get_epoch(i, infos->actual_height);
            if (my_new_epoch != NULL && my_new_epoch->block_data.height)
            {
                if (my_new_epoch->block_data.is_prev_block_valid) {
                    Block* lastblock = get_block(height+1);
                    update_wallet_with_block(*lastblock);
                    free_block(lastblock);
                }
                write_block_file(*my_new_epoch);
                infos->actual_height++;
                MANAGERMSG
                printf("Block %lu is added in the blockchain!\n", my_new_epoch->block_data.height);
                free_block(my_new_epoch);
                break;
            }
        }
        delete_epochs(infos->actual_height);
        if (infos->is_validator > 0)
        {
            infos->as_epoch = 0;
            infos->validator_id = i_am_commitee_member();
        }
    }


    return 0;
}

int read_send_pending_transaction_list(int fd, infos_st *infos)
{
    size_t nbtxids = 0;
    read(fd, &nbtxids, sizeof(size_t));
    time_t txids[500];
    read(fd, txids, sizeof(time_t) * nbtxids);
    CLIENTMSG
    printf("Receive a list of %lu transactions!\n", nbtxids);
    for (size_t i = 0; i < nbtxids; i++)
    {
        char temp[50];
        sprintf(temp, "data/pdt/%ld", *(txids + i));
        if (access(temp, F_OK))
        {
            // file doesn't exists
            send_get_pending_transaction(fd, *(txids + i));
            read_header(fd, infos);
        }
    }
    return nbtxids;
}

int read_send_pending_transaction(int fd, infos_st* infos)
{
    char dir[256];
    char temp[1024];
    size_t bc_size;
    time_t txid;
    ssize_t r = read(fd, &txid, sizeof(time_t));
    if (r != sizeof(time_t))
        return -1;
    r = read(fd, &bc_size, sizeof(size_t));
    if (r != sizeof(size_t))
        return -1;

    snprintf(dir, 256, "data/pdt/%ld", txid);
    if (access(dir, F_OK) == 0)
    {
        // file exists
        return txid;
    }

    int transfile = open(dir, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (transfile == -1)
    {
        CLIENTMSG
        printf("Can't create transaction %ld file\n", txid);
        return -1;
    }

    while ((r = read(fd, temp, bc_size > 1024 ? 1024 : bc_size)) && bc_size > 0)
    {
        if (r == -1)
            errx(EXIT_FAILURE, "Can't read transaction %ld in connection fd: %i", txid, fd);
        safe_write(transfile, temp, r);
        bc_size -= r;
    }
    if (bc_size > 0)
    {
        WARNINGMSG("Failed to read all the block!")
    }

    close(transfile);
    SERVERMSG
    printf("Recived read_pending_transaction %lu \
    transaction in connection fd: %i\n",
           txid, fd);
    update_pdt(1);

        // CREATE NEW EPOCH
    if (infos->is_validator == 1 && infos->pdt > 0)
    {
        infos->is_validator++;

        Validate();
    }

    return txid;
}

int read_get_pending_transaction(int fd)
{

    time_t txid;
    read(fd, &txid, sizeof(time_t));
    send_send_pending_transaction(fd, txid);
    return txid;
}
