#include "../../src/network/network.h"
#include "../../src/network/client.h"

int main()
{
    printf("Testing client:\n");

    // Init client lists
    
    set_neighbours();

    int sockfd;

    set_neighbours();
    sockfd = connect_to_network(0);
    if (sockfd == -1)
    {
        // You are the first node to the peer-to-peer network
        return -1;
    }
    wait_server_header(sockfd);

    return 0;
}