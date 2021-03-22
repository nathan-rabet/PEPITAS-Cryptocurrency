#include <stdlib.h>
#include <math.h>

void *add_bits_n(void *uint1, void *uint2, unsigned short n)
{
    char nb_i = 0;
    while (n >= 64)
    {
        nb_i++;
        n <<= 1;
    }

    void * res = malloc(n);
    for (char i = 0; i < nb_i; i++)
    {
        unsigned long val1 = *((unsigned long *)uint1 + 64 * (nb_i - 1 - i));
        unsigned long val2 = *((unsigned long *)uint2 + 64 * (nb_i - 1 - i));

        unsigned long overflow;
        __builtin_uaddl_overflow(
            *((unsigned long *)uint1 + 64 * (nb_i - 1 - i)),
            *((unsigned long *)uint2 + 64 * (nb_i - 1 - i)),
            &overflow);
        if (overflow && i != nb_i)
        {
            *((unsigned long *)res + 64 * (nb_i - 2 - i)) += 1;
        }

        *((unsigned long *)res + 64 * (nb_i - 1 - i)) = val1 + val2;
    }

    return res;
}
/*
void *sub_bits_n(void *uint1, void *uint2, unsigned short n)
{
    // TODO
}

void *mult_bits_n(void *uint1, void *uint2, unsigned short n)
{
    // TODO
}

void *div_bits_n(void *numerator, void *denominator, unsigned short n)
{
    // TODO
}

void *mod_bits_n(void *numerator, void *denominator, unsigned short n)
{
    // TODO
}
*/