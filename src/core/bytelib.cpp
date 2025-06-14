#include "gameboy-emulator/core/bytelib.hpp"

#include <iostream>

namespace emulator
{

uint16_t bytes_to_16b(const uint8_t &msB, const uint8_t &lsB)
{
    return ((int)msB << 8) | lsB;
}

uint16_t byte_to_16b(const uint8_t &b)
{
    if (b >> 7)
    {
        return b | 0xFF00;
    }
    return b;
}

void set_bit(int i, uint8_t &b)
{
    if (i < 8)
    {
        uint8_t x = 1 << i;
        b = b | x;
    }
}

void set_bit(int i, uint16_t &b)
{
    if (i < 16)
    {
        uint16_t x = 1 << i;
        b = b | x;
    }
}

void reset_bit(int i, uint8_t &b)
{
    if (i < 8)
    {
        uint8_t x = 1 << i;
        x = ~x;
        b = b & x;
    }
}

void reset_bit(int i, uint16_t &b)
{
    if (i < 16)
    {
        uint16_t x = 1 << i;
        x = ~x;
        b = b & x;
    }
}

} // namespace emulator
