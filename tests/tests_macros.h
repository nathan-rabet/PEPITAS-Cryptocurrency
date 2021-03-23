#define DEBUG() printf("Debugging '%s'\n", __FUNCTION__);

#define TEST_PASSED(name) printf("\033[0;32mTest ⟼ '%s' success [✓]\033[0m\n",name);

#define TEST_FAILED(name, reason) printf("\033[0;31mTest '%s' failed [✗]\n\t⤷ Reason : %s\033[0m\n",name, reason); exit(1)