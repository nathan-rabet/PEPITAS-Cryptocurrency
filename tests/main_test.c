#include <assert.h>
#include "../src/network/network.h"

int main(int argc, char** argv)
{
    ConnectionToNetwork(STATIC_DNS, STATIC_PORT);

    return 0;
}