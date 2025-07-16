#include "gameboy-emulator/core/bytelib.hpp"

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

void set_bit(uint8_t i, uint8_t &b)
{
    i = 1 << i;
    b = b | i;
}

void set_bit(uint16_t i, uint16_t &b)
{
    i = 1 << i;
    b = b | i;
}

void reset_bit(uint8_t i, uint8_t &b)
{
    i = 1 << i;
    i = ~i;
    b = b & i;
}

void reset_bit(uint16_t i, uint16_t &b)
{
    i = 1 << i;
    i = ~i;
    b = b & i;
}

bool check_bit(uint8_t i, const uint8_t &b)
{
    return (b >> i) & 1;
}

} // namespace emulator
