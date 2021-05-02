
#include "network/server.h"

int main()
{
    printf("Testing init_server()\n");
    infos_st *infos = malloc(sizeof(infos_st));
    infos->actual_height = 0;
    infos->is_sychronize = 0;
    infos->serv_type = NODESERVER;
    return init_server(infos);
}