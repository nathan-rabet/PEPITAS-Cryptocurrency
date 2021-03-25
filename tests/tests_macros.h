#include <stdio.h>

#define DEBUG(function) function();

#define TEST_PASSED(name) printf("\033[0;32mTEST → '%s' success [✓]\033[0m\n",name);

#define TEST_FAILED(name, reason) printf("\033[0;31mTEST '%s' failed [✗]\n\t⤷ REASON : %s\033[0m\n",name, reason); exit(1)