#include "network/server.h"
#include "network/client.h"
#include "cryptosystem/signature.h"
#include "core/blockchain/block.h"
#include <time.h>

int main()
{
    get_my_node()->neighbours[0].family = AF_INET;
    get_my_node()->neighbours[0].hostname = "728.191.117.199";

    get_my_node()->neighbours[1].family = AF_INET6;
    get_my_node()->neighbours[1].hostname = "2a02:842a:222:701:6a5:b521:f6bd:996b";
    return init_server();
}