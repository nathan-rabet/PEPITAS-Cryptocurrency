#include "core/cryptosystem/rsa.h"
#include <stdio.h>
#include <time.h>

char IsPrime(u_int64_t n)
{
    if (n <= 3)
        return n > 1;
    if ((n % 2 == 0) || (n % 3 == 0))
        return 0;
    u_int64_t i = 5;
    while (i * i <= n)
    {
        if ((n % i == 0) || (n % (i + 2) == 0))
            return 0;
        i += 6;
    }
    return 1;
}

u_int64_t PGCD_extend(u_int64_t a, u_int64_t b)
{
    u_int64_t s1 = 1;
    u_int64_t s2 = 0;
    u_int64_t t1 = 0;
    u_int64_t t2 = 1;
    u_int64_t q;
    
    u_int64_t t;
    u_int64_t s;
    u_int64_t r;


    while (b != 0)
    {
        q = a/b;
        r = a%b;
        a = b;
        b = r;

        s = s1-q*s2;
        s1 = s2;
        s2 = s;
        t = t1-q*t2;
        t1 = t2;
        t2 = t;
    }

    return s1;
}

// PUBLIC KEY : (n, e)
// PRIVATE KEY: (n, d)
char* generate_key()
{
    srandom(time(NULL));
    u_int64_t p;
    u_int64_t q;
    u_int64_t n;
    u_int32_t e;
    u_int64_t phi;
    u_int64_t d;

    do
    {
        p = (random()+1) * (random()+7);
        p &= 0x000000007FFFFFFF;
        p |= 0x0000000080000000;
    }while(p < RAND_MAX && IsPrime(p));
    do
    {
        q = (random()+1) * (random()+7);
        q &= 0x000000007FFFFFFF;
        q |= 0x0000000080000000;
    }while(q < RAND_MAX && IsPrime(q));

    // SWAP TO GET P > Q
    if (p < q)
    {
        n = p;
        p = q;
        q = n;
    }

    n = p*q;
    phi = (p - 1)*(q - 1);
    e = 65537;
    d = PGCD_extend(e, phi);
    printf("RSA KEYS:\np: %0lx | q: %0lx | n: %lu\nphi: %lu\ne: %u\nd: %lu\n", p,q,n,phi,e,d);
    return NULL;
}