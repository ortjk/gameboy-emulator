#include "gameboy-emulator/core/memory.hpp"

namespace emulator
{

uint8_t Memory::registers[65536] = {};

uint8_t *Memory::get_8b(const uint16_t &address)
{
    return &registers[address];
}

uint16_t *Memory::get_16b(const uint16_t &address)
{
    return reinterpret_cast<uint16_t *>(&registers[address]);
}

#ifdef CMAKE_BUILD_TESTING

void Memory::write(const uint8_t &b, const uint16_t &address)
{
    registers[address] = b;
}

void Memory::write(const uint16_t &b, const uint16_t &address)
{
    uint8_t msb = static_cast<uint8_t>(b >> 8);
    uint8_t lsb = static_cast<uint8_t>(b & 0xFF);
    registers[address] = lsb;
    registers[address+1] = msb;
}

void Memory::write(const uint8_t &msb, const uint8_t &lsb, const uint16_t &address)
{
    registers[address] = lsb;
    registers[address+1] = msb;
}

#endif

} // namespace emulator
