#include <stdlib.h>
#include <stdio.h>
#include "tests_macros.h"
#include "misc/bits_n.h"

void add_bits_n_test()
{
    DEBUG();

    // overflow
    bits256 val1 = {0};
    bits256 val2 = {0};

    val1.bits[0] = 0;
    val1.bits[1] = 0;
    val1.bits[2] = 0;
    val1.bits[3] = 42;

    val2.bits[0] = 0;
    val2.bits[1] = 0;
    val2.bits[2] = 0;
    val2.bits[3] = 69;

    bits256 res = *((bits256 *)add_bits_n(&val1, &val2, 256));

    if (res.bits[3] == 111)
    {
        TEST_PASSED("Low bits addition");
    }
    else
    {
        // char err_msg[256];
        //vsprintf(err_msg,"Lower bit is equal to '%llu' but must be 111",res.bits[3]);
        TEST_FAILED("Low bits addition", "Lower bits are not equal to 111");
    }
}
