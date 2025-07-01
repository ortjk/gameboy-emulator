#include "gameboy-emulator/core/memory.hpp"

#include <stdexcept>

namespace emulator
{

uint8_t Memory::_registers[65536] = {};
uint8_t Memory::_dmg[256] = {};

uint8_t *Memory::get_8b(const uint16_t &address)
{
    if (address < 0x100 && _registers[0xFF50] == 0x00) // check if dmg rom is enabled
    {
        return &_dmg[address];
    }

    return &_registers[address];
}

uint16_t *Memory::get_16b(const uint16_t &address)
{
    if (address < 0x100 && _registers[0xFF50] == 0x00)
    {
        if (address == 0xFF)
        {
            throw std::invalid_argument("16 bit integer requested spans dmg and rom bank");
        }
        else
        {
            return reinterpret_cast<uint16_t *>(&_dmg[address]);
        }
    }

    return reinterpret_cast<uint16_t *>(&_registers[address]);
}

uint8_t *Memory::get_8b_registers(const uint16_t &address)
{
    return &_registers[address];
}

uint8_t *Memory::get_8b_dmg(const uint8_t &address)
{
    return &_dmg[address];
}

#ifdef CMAKE_BUILD_TESTING

void Memory::write(const uint8_t &b, const uint16_t &address)
{
    _registers[address] = b;
}

void Memory::write(const uint16_t &b, const uint16_t &address)
{
    uint8_t msb = static_cast<uint8_t>(b >> 8);
    uint8_t lsb = static_cast<uint8_t>(b & 0xFF);
    _registers[address] = lsb;
    _registers[address+1] = msb;
}

void Memory::write(const uint8_t &msb, const uint8_t &lsb, const uint16_t &address)
{
    _registers[address] = lsb;
    _registers[address+1] = msb;
}

#endif

} // namespace emulator
