#include "network/server.h"
#include "network/client.h"
#include "cryptosystem/signature.h"
#include "core/blockchain/block.h"
#include <time.h>

int main()
{
    get_my_node()->neighbours[0].family = AF_INET;
    get_my_node()->neighbours[0].hostname = "78.191.117.199";
    return init_server();
}