
#include "network/server.h"
#include "network/client.h"

int main()
{
    if (set_neighbour(NULL, 0) == 0)
        errx(EXIT_FAILURE,"Failed to load neighbours\n");
    
    return init_server();
}