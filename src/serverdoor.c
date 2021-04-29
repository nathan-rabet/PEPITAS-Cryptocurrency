#include "network/server.h"
#include "network/client.h"
#include "cryptosystem/signature.h"
#include "blockchain/block.h"
#include <time.h>

/*
    Server to connect for the first time nodes
*/
int main()
{
    printf("Starting door server\n");
    load_neighbours(IM_SERVER);
    print_neighbours(IM_SERVER, 0);
    infos_st* infos = malloc(sizeof(infos_st));
    infos->serv_type = DOORSERVER;
    init_server(infos);
    return 0;
}