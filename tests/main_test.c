#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"


int main()
{
    // // Init client lists
    // get_my_node();

    // //
    // set_neighbour(NULL, 0);

    get_my_node();

    for (size_t i = 0; i < 10; i++)
    {
        get_my_node()->neighbours[i].hostname = "192.168.1.1";
        get_my_node()->neighbours[i].family = AF_INET;
    }    

    init_server();

    return 0;
}