#include <stdlib.h>
#include <math.h>

void *add_bits_n(void *uint1, void *uint2, unsigned short n)
{
    char nb_i = 1;
    while (n >= 64)
    {
        nb_i++;
        n >>= 1;
    }

    void *res = calloc(n, 1);
    for (char i = 0; i < nb_i; i++)
    {
        unsigned long val1 = *(unsigned long *)(uint1 + sizeof(unsigned long) * (nb_i - 1 - i));
        unsigned long val2 = *(unsigned long *)(uint2 + sizeof(unsigned long) * (nb_i - 1 - i));

        unsigned long tmp_res;
        if (__builtin_uaddl_overflow(val1, val2, &tmp_res) && i != nb_i)
        {
            *((unsigned long *)res + (nb_i - 2 - i)) += 1;

            // 
        }
        if (tmp_res == 0xFFFFFFFFFFFFFFFF && *((unsigned long *)res + (nb_i - 1 - i)) == 1
            && i != nb_i)
            *((unsigned long *)res + (nb_i - 2 - i)) += 1;
        *((unsigned long *)res + (nb_i - 1 - i)) += tmp_res;
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