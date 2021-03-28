
#include "network/server.h"
#include "network/client.h"

int main()
{
    set_neighbour(NULL, 0);
    get_my_node()->neighbours->hostname = "192.168.1.1";
    get_my_node()->neighbours->family = AF_INET;
    
    return init_server();
}