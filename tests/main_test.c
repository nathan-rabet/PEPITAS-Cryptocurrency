#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"
#include <thread.h>


void * server_handler(void * arg) {
    return (void *) init_server();
}

int main()
{
    get_my_node();

    pthread_t server_instance;
    pthread_create(&server_instance, NULL, server_handler, NULL);

    return 0;
}