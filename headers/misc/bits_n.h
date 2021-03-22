#include <stdlib.h>

// 64 bits * 4 = 256 bits
typedef struct bits256
{
    unsigned long bits[4];
} bits256;

// 64 bits * 8 = 512 bits
typedef struct bits512
{
    unsigned long bits[8];
} bits512;

// 64 bits * 16 = 1024 bits
typedef struct bits1024
{
    unsigned long bits[16];
} bits1024;

// 64 bits * 32 = 2048 bits
typedef struct bits2048
{
    unsigned long bits[32];
} bits2048;

// 64 bits * 64 = 4096 bits
typedef struct bits4096
{
    unsigned long bits[64];
} bits4096;

// 64 bits * 128 = 8192 bits
typedef struct bits8192
{
    unsigned long bits[128];
} bits8192;

/**
 * @brief Add two 'n' bits numbers
 * 
 * @param uint1 The first 'n' bits number
 * @param uint2 The second 'n' bits number
 * @param n The number of bits, must be a multiple of 64 and must start at 256 bits.
 * @return void* 
 */
void *add_bits_n(void *uint1, void *uint2, unsigned short n);

/**
 * @brief Subsrtact two 'n' bits numbers
 * 
 * @param uint1 The first 'n' bits number
 * @param uint2 The second 'n' bits number
 * @param n The number of bits, must be a multiple of 64 and must start at 256 bits.
 * @return void* 
 */
void *sub_bits_n(void *uint1, void *uint2, unsigned short n);

/**
 * @brief Multiply two 'n' bits numbers
 * 
 * @param uint1 The first 'n' bits number
 * @param uint2 The second 'n' bits number
 * @param n The number of bits, must be a multiple of 64 and must start at 256 bits.
 * @return void* 
 */
void *mult_bits_n(void *uint1, void *uint2, unsigned short n);

/**
 * @brief Divide two 'n' bits numbers
 * 
 * @param numerator The 'n' bits numerator
 * @param denominator The 'n' bits denominator
 * @param n The number of bits, must be a multiple of 64 and must start at 256 bits.
 * @return void* 
 */
void *div_bits_n(void *numerator, void *denominator, unsigned short n);

/**
 * @brief Modulus two 'n' bits numbers
 * 
 * @param numerator The 'n' bits numerator
 * @param denominator The 'n' bits denominator
 * @param n The number of bits, must be a multiple of 64 and must start at 256 bits.
 * @return void* 
 */
void *mod_bits_n(void *numerator, void *denominator, unsigned short n);
