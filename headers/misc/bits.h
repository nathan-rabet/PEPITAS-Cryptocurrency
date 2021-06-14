#ifndef BITS_H
#define BITS_H

/**
 * @brief Read a single bit of data
 * 
 * @param buffer The buffer to read
 * @param index The index (in bit) to read the bit
 * @return char 
 */
static inline char read_single_bit(unsigned char* buffer, unsigned int index)
{
    return ((buffer[index / 8] >> (7 - index % 8)) & 1);
}

#endif