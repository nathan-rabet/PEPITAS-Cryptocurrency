#ifndef RSA_H
#define RSA_H

#include <stdlib.h>

// YES OR NO PRIME
char IsPrime(u_int64_t n);


u_int32_t Find_e(u_int64_t phi);

u_int64_t PGCD(u_int64_t a, u_int64_t b);

u_int64_t PGCD_extend(u_int64_t a, u_int64_t b);

// GENERATE KEY
char* generate_key();

#endif