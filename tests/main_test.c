#include <assert.h>
#include "../src/network/network.h"

int network_test()
{
    int sockfd;

    set_neighbours();
    sockfd = connect_to_network(0);
    if (sockfd == -1)
    {
        // You are the first node to the peer-to-peer network
    }
    get_client_data(sockfd);

    return 0;
}
int main()
{
    init_server();

    return 0;
}