#include <assert.h>
#include "../src/network/client.h"
#include "../src/misc/safe.h"
#include "../src/cryptosystem/rsa.h"

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