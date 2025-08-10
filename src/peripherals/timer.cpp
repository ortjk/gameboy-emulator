#include "gameboy-emulator/peripherals/timer.hpp"

#include "gameboy-emulator/core/bytelib.hpp"
#include "gameboy-emulator/core/cpu.hpp"
#include "gameboy-emulator/core/memory.hpp"

namespace emulator {

const uint8_t *Timer::tma = &Memory::registers[0xff06];
const uint8_t *Timer::tac = &Memory::registers[0xff07];

uint8_t *Timer::div = &Memory::registers[0xff04];
uint8_t *Timer::tima = &Memory::registers[0xff05];

const uint8_t Timer::clk_sel[4] = {
    64, // 256 M-cycles
    1,  // 4 M-cycles
    4,  // 16 M-cycles
    16  // 64 M-cycles
};

uint8_t Timer::div_d = 0;
uint8_t Timer::tima_d = 0;

void Timer::increment_div()
{
    if (++div_d >= 16) // check clock divider
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

        // request interrupt
        CPU::interrupt(TIMER_INT);
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
