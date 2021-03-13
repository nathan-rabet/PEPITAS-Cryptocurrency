#include <assert.h>
#include "../src/network/client.h"
#include "../src/misc/safe.h"

int network_test()
{
    int sockfd;

    set_neighbours(NULL, 0);
    sockfd = connect_to_network(0);
    if (sockfd == -1)
    {
        // You are the first node to the peer-to-peer network
    }
    wait_server_header(sockfd);

    return 0;
}
int main()
{
    char *buffer;
    size_t nb;
    ssize_t oui = safe_read(STDIN_FILENO, (void *)&buffer, &nb);
    if (oui == -1)
        errx(EXIT_FAILURE, "SUCE MA QUE");
    write(STDOUT_FILENO, buffer, oui);

    return 0;
}