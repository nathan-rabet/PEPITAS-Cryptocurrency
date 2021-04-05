#ifndef TESTS_MACROS_H
#define TESTS_MACROS_H
#include <stdio.h>

#define DEBUG(function) function()

#define LOG(str...)           \
    printf("\033[0;34m[?] "); \
    printf(str);              \
    printf("\033[0m\n")

#define TEST_PASSED(name...)          \
    printf("\033[0;32m[✓] TEST → '"); \
    printf(name);                     \
    printf("' success\033[0m\n")

#define TEST_FAILED(name, reason...)                               \
    printf("\033[0;31m[✗] TEST '%s' failed\n\t⤷ REASON : ", name); \
    printf(reason);                                                \
    printf("\033[0m\n");                                           \
    exit(1)

#define TEST_WARNING(name, reason...)                          \
    printf("\033[0;33m[!] WARNING '%s'\n\t⤷ BECAUSE : ", name); \
    printf(reason);                                            \
    printf("\033[0m\n")

#endif