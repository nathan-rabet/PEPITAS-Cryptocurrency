#include "network/server.h"
#include "network/client.h"
#include "cryptosystem/signature.h"
#include "core/blockchain/block.h"
#include <time.h>

/*
    Server to connect for the first time nodes
*/
int main()
{
    printf("Starting door server\n");
    load_neighbours(IM_SERVER);
    print_neighbours(IM_SERVER, 0);
    return init_server(DOORSERVER);
}