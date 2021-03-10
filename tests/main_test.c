#include <assert.h>
#include "../src/network/network.h"

int client()
{
    int sockfd;

    ClientData cl;
    sockfd = connect_to_network(&cl,1);
    if (sockfd == -1)
    {
        // You are the first node to the peer-to-peer network
    }
    get_client_data(sockfd);

    return 0;
}
int main()
{
    int fd;

    fd = fork();
    if (fd == -1)
        errx(EXIT_FAILURE, "Counld not fork!");
    if (fd != 0)
    {
        im_server();
    }
    //return Client();
    while (1)
    {
        /* code */
    }

    return 0;
}