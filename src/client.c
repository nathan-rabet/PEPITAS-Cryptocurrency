#include <signal.h>
#include <stdlib.h>
#include "tests_macros.h"

#include "network/network.h"
#include "network/client.h"
#include "network/server.h"
#include "network/send_data.h"
#include "network/get_data.h"

int main()
{
    Block* block = malloc(sizeof(Block));
    verify_block_signature(block);
    return 0;
}