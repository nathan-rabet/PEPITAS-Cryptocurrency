char read_single_bit(unsigned char* buffer, unsigned int index)
{
    unsigned char c = buffer[index / 8]; //getting the byte which contains the bit
    unsigned int bit_position = index % 8; //getting the position of that bit within the byte

    return ((c >> (7 - bit_position)) & 1);
    //shifting that byte to the right with (7 - bit_position) will move the bit whose value you want to know at "the end" of the byte.
    //then, by doing bitwise AND with the new byte and 1 (whose binary representation is 00000001) will yield 1 or 0, depending on the value of the bit you need.
}