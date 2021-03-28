#include <stdio.h>

#define DEBUG(function) function()

#define LOG(str...) printf("\033[0;34m"); printf(str); printf("\033[0m")

#define TEST_PASSED(name) printf("\033[0;32mTEST → '%s' success [✓]\033[0m\n", name)

#define TEST_FAILED(name, reason...)                               \
    printf("\033[0;31mTEST '%s' failed [✗]\n\t⤷ REASON : ", name); \
    printf(reason);                                                \
    printf("\033[0m\n");                                           \
    exit(1)