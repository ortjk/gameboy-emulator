#include "gameboy-emulator/core/memory.hpp"

namespace emulator
{

uint8_t Memory::registers[65536] = {};
uint8_t Memory::dmg[256] = {};

bool Memory::wrote = false;
uint16_t Memory::last_addr = 0x0000;

uint16_t *Memory::get_16b(const uint16_t &address)
{
    return reinterpret_cast<uint16_t *>(&registers[address]);
}

void Memory::unmap_dmg()
{
    uint8_t t = 0;
    for (int i = 0; i < 256; i++)
    {
        t = registers[i];
        registers[i] = dmg[i];
        dmg[i] = t;
    }
}

#ifdef CMAKE_BUILD_TESTING

void Memory::write(const uint8_t &b, const uint16_t &address)
{
    if (address < 0x100 && _registers[0xFF50] == 0x00)
    {
        _dmg[address] = b;
    }
    _registers[address] = b;
}

void Memory::write(const uint16_t &b, const uint16_t &address)
{
    uint8_t msb = static_cast<uint8_t>(b >> 8);
    uint8_t lsb = static_cast<uint8_t>(b & 0xFF);

    if (address < 0x100 && _registers[0xFF50] == 0x00)
    {
        _dmg[address] = lsb;
        _dmg[address+1] = msb;
    }

    _registers[address] = lsb;
    _registers[address+1] = msb;
}

void Memory::write(const uint8_t &msb, const uint8_t &lsb, const uint16_t &address)
{
    if (address < 0x100 && _registers[0xFF50] == 0x00)
    {
        _dmg[address] = lsb;
        _dmg[address+1] = msb;
    }
    
    _registers[address] = lsb;
    _registers[address+1] = msb;
}

#endif

} // namespace emulator
