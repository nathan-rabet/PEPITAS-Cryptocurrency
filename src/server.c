#include "network/server.h"
#include "network/client.h"
#include "cryptosystem/signature.h"
#include "blockchain/block.h"
#include <time.h>

int main()
{
    infos_st *infos = malloc(sizeof(infos_st));
    infos->actual_height = 0;
    infos->is_sychronize = 0;
    infos->serv_type = NODESERVER;
    print_neighbours(IM_SERVER, 1);
    init_server(infos);
    return 0;
}