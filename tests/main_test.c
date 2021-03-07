#include <assert.h>
#include "../src/network/network.h"




int ImServer()
{
    printf("I'am server\n");
    return InitServer(STATIC_DNS, "25565");
}

int Client()
{
    int sockfd;

    sockfd = ConnectionToNetwork(STATIC_DNS, STATIC_PORT);
    if (sockfd == -1)
    {
        // You are the first node to the peer-to-peer network
    }
    struct Clientdata cl;
    GetClientData(sockfd, &cl, 0, 0);

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
        ImServer();
    }
    //return Client();
    while (1)
    {
        /* code */
    }
    
    return 0;
}