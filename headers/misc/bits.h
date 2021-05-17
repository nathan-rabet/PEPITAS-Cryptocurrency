#ifndef BITS_H
#define BITS_H

inline char read_single_bit(unsigned char* buffer, unsigned int index)
{
    return ((buffer[index / 8] >> (7 - index % 8)) & 1);
}

#endif