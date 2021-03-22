#define DEBUG() printf("Debugging %ms:%s\n"__FILE__, __FUNCTION__);

#define TEST_PASSED(name) printf("\033[0;33mTest '%s' success [✓]\033[0m\n",name);

#define TEST_FAILED(name, reason) printf("\033[0;33mTest '%s' failed [✗]\n\tReason : %s\033[0m\n",name, reason); exit(1)