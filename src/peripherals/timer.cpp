#include "gameboy-emulator/peripherals/timer.hpp"

#include "gameboy-emulator/core/bytelib.hpp"
#include "gameboy-emulator/core/memory.hpp"

namespace emulator {

const uint8_t *Timer::tma = &Memory::registers[0xff06];
const uint8_t *Timer::tac = &Memory::registers[0xff07];

uint8_t *Timer::div = &Memory::registers[0xff04];
uint8_t *Timer::tima = &Memory::registers[0xff05];
uint8_t *Timer::_if = &Memory::registers[0xff0f];

const uint8_t Timer::clk_sel[4] = {
    255, // 256 M-cycles
    3,  // 4 M-cycles
    15,  // 16 M-cycles
    63  // 64 M-cycles
};

uint8_t Timer::div_d = 0;
uint8_t Timer::tima_d = 0;

void Timer::increment_div()
{
    if (++div_d >= 63) // check clock divider
    {
        div_d = 0;
        (*div)++;
    }
}

void Timer::increment_tima()
{
    if (!check_bit(2, *tac)) // check tac:Enable
    {
        return;
    }

    if (++tima_d < clk_sel[*tac & 0x3]) // check clock divider
    {
        return;
    }
    tima_d = 0;

    if (*tima == 0xFF) // overflow will occur
    {
        // set tima to tma
        *tima = *tma;

        // request timer interrupt
        set_bit(2, *_if); 
    }
    else // no overflow will occur 
    {
        (*tima)++;
    }
}

void Timer::tick()
{
    increment_div();
    increment_tima();
}

} // namespace emulator
