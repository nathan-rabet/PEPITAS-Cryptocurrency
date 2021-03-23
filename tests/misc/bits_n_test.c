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
        char err_msg[256];
        sprintf(err_msg, "Lower 64 bits are equal to '%lx' but must be 'Ox6f'", res.bits[3]);
        TEST_FAILED("Low bits addition", err_msg);
    }

    val1.bits[0] = 0;
    val1.bits[1] = 0;
    val1.bits[2] = 0;
    val1.bits[3] = 0xffffffffffffffff;

    val2.bits[0] = 0;
    val2.bits[1] = 0;
    val2.bits[2] = 0;
    val2.bits[3] = 1;

    res = *((bits256 *)add_bits_n(&val1, &val2, 256));

    if (res.bits[2] == 1 && res.bits[3] == 0)
    {
        TEST_PASSED("Overflow low bits addition");
    }
    else
    {
        char err_msg[256];
        sprintf(err_msg, "res.bits[2] = '%016lx' (must be 1), res.bits[3] = '%016lx' (must be 0)", res.bits[2], res.bits[3]);
        TEST_FAILED("Overflow low bits addition", err_msg);
    }

    val1.bits[0] = 0xffffffffffffffff;
    val1.bits[1] = 0;
    val1.bits[2] = 0;
    val1.bits[3] = 0;

    val2.bits[0] = 1;
    val2.bits[1] = 0;
    val2.bits[2] = 0;
    val2.bits[3] = 0;

    res = *((bits256 *)add_bits_n(&val1, &val2, 256));

    if (res.bits[0] == 0 && res.bits[1] == 0)
    {
        TEST_PASSED("Overflow high bits addition");
    }
    else
    {
        char err_msg[256];
        sprintf(err_msg, "res.bits[0] = '%016lx' (must be 0), res.bits[1] = '%016lx' (must be 0)", res.bits[0], res.bits[1]);
        TEST_FAILED("Overflow low bits addition", err_msg);
    }

    val1.bits[0] = 0;
    val1.bits[1] = 0;
    val1.bits[2] = 0xffffffffffffffff;
    val1.bits[3] = 1;

    val2.bits[0] = 0;
    val2.bits[1] = 0;
    val2.bits[2] = 0;
    val2.bits[3] = 0xffffffffffffffff;

    res = *((bits256 *)add_bits_n(&val1, &val2, 256));

    if (res.bits[0] == 0 && res.bits[1] == 1 && res.bits[2] == 0 && res.bits[3] == 0)
    {
        TEST_PASSED("Simple double overflow");
    }
    else
    {
        char err_msg[256];
        sprintf(err_msg, "res.bits[0] = '%016lx' (must be 0), res.bits[1] = '%016lx' (must be 0)", res.bits[0], res.bits[1]);
        TEST_FAILED("Simple double overflow", err_msg);
    }

    val1.bits[0] = 0xffffffffffffffff;
    val1.bits[1] = 0xffffffffffffffff;
    val1.bits[2] = 0xffffffffffffffff;
    val1.bits[3] = 0xffffffffffffffff;

    val2.bits[0] = 0xffffffffffffffff;
    val2.bits[1] = 0xffffffffffffffff;
    val2.bits[2] = 0xffffffffffffffff;
    val2.bits[3] = 0xffffffffffffffff;

    res = *((bits256 *)add_bits_n(&val1, &val2, 256));

    if (res.bits[0] == 0xffffffffffffffff && res.bits[1] == 0xffffffffffffffff && res.bits[2] == 0xffffffffffffffff && res.bits[3] == 0xfffffffffffffffe)
    {
        TEST_PASSED("Cursed double overflow");
    }
    else
    {
        char err_msg[256];
        sprintf(err_msg, "res.bits[0] = '%016lx' (must be 0xffffffffffffffff), res.bits[1] = '%016lx' (must be 0xffffffffffffffff), res.bits[2] = '%016lx' (must be 0xffffffffffffffff), res.bits[3] = '%016lx' (must be 0xfffffffffffffffe)", res.bits[0], res.bits[1], res.bits[2], res.bits[3]);
        TEST_FAILED("Cursed double overflow", err_msg);
    }

    val1.bits[0] = 0x16092001;
    val1.bits[1] = 0x30012001;
    val1.bits[2] = 0xffffffffffffffff;
    val1.bits[3] = 0x1234567891234567;

    val2.bits[0] = 0xdeadbeefdeadbeef;
    val2.bits[1] = 0xdeadbeef;
    val2.bits[2] = 0xff78bacbac;
    val2.bits[3] = 0x0f67;

    res = *((bits256 *)add_bits_n(&val1, &val2, 256));

    if (res.bits[0] == 0xdeadbeeff4b6def0 
    && res.bits[1] == 0x10eaedef1
    && res.bits[2] == 0xff78bacbab
    && res.bits[3] == 0x12345678912354CE)
    {
        TEST_PASSED("Usual calculus (with overflow)");
    }
    else
    {
        char err_msg[256];
        sprintf(err_msg, "res.bits[0] = '%016lx' (must be 0xdeadbeeff4b6def0), res.bits[1] = '%016lx' (must be 0x10eaedef1), res.bits[2] = '%016lx' (must be 0xff78bacbab), res.bits[3] = '%016lx' (must be 0x12345678912354CE)", res.bits[0], res.bits[1], res.bits[2], res.bits[3]);
        TEST_FAILED("Usual calculus (with overflow)", err_msg);
    }
}
