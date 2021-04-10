#include "network/server.h"
#include "network/client.h"
#include "cryptosystem/signature.h"
#include "core/blockchain/block.h"
#include <time.h>

int main()
{
    set_neighbour("2a02:842a:222:701:6a5:b521:f6bd:996b", AF_INET6);
    set_neighbour("2a02:842a:222:701:6a5:b521:f6bd:996b", AF_INET6);
    set_neighbour("2a02:842a:222:701:6a5:b521:f6bd:996b", AF_INET6);
    set_neighbour("2a02:842a:222:701:6a5:b521:f6bd:996b", AF_INET6);
    print_neighbours(1);
    return init_server(NODESERVER);
}