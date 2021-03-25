#include <assert.h>
#include <stdio.h>
#include "network/client.h"
#include "misc/safe.h"
#include "core/cryptosystem/coding.h"
#include "core/cryptosystem/rsa.h"
#include "core/blockchain/wallet.h"
#include <openssl/rsa.h>

int network_test()
{
    int sockfd;

    set_neighbours(NULL, 0);
    sockfd = connect_to_network(0);
    if (sockfd == -1)
    {
        // You are the first node to the peer-to-peer network
    }
    wait_header(sockfd);

    return 0;
}
int main()
{
    Wallet* w = get_my_wallet();
    generate_key();


    char* msg = "Coucou bande de nouilles!";
    char* signature;
    sign_message(msg, &signature);
    printf("Signature: ");
    for (size_t i = 0; i < strlen(signature); i++)
    {
        printf("%c", signature[i]);
    }
    printf("\n");
    if (verify_sign(msg, signature))
    {
        printf("Signature is true!\n");
    }
    else
    {
        printf("Signature is false. :(\n");
    }
    
    return 0;
}