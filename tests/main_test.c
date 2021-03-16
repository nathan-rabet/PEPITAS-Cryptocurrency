#include <assert.h>
#include "network/client.h"
#include "misc/safe.h"
#include "core/cryptosystem/rsa.h"

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
    generate_key();

    return 0;
}