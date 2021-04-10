#include "network/server.h"
#include "network/client.h"
#include "cryptosystem/signature.h"
#include "core/blockchain/block.h"
#include <time.h>

int main()
{
    print_neighbours(IM_SERVER, 1);
    return init_server(NODESERVER);
}