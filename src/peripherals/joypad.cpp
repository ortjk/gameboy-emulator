#include "gameboy-emulator/peripherals/joypad.hpp"

#include "gameboy-emulator/core/bytelib.hpp"
#include "gameboy-emulator/core/memory.hpp"

namespace emulator {

uint8_t *Joypad::joyp = &Memory::registers[0xff00];
uint8_t *Joypad::_if = &Memory::registers[0xff0f];

void Joypad::tick(const uint8_t &buttons, const bool &joypad_int)
{
    uint8_t sel = (*joyp) >> 4; // upper nibble of joyp

    if (sel == 0x3) // neither selected
    {
        *joyp |= 0x0F;
    }
    else if (sel == 0x2) // select d-pad (buttons lower nibble)
    {
        *joyp = (*joyp & 0xF0) | (buttons & 0x0F);
    }
    else if (sel == 0x1) // select buttons (buttons upper nibble)
    {
        *joyp = (*joyp & 0xF0) | (buttons >> 4);
    }
    else  // both selected?
    {
        
    }

    if (joypad_int)
    {
        // request joypad interrupt
        set_bit(4, *_if);
    }
}

} // namepace emulator