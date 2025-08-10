#include "gameboy-emulator/peripherals/serial.hpp"

#include "gameboy-emulator/core/cpu.hpp"
#include "gameboy-emulator/core/bytelib.hpp"
#include "gameboy-emulator/core/memory.hpp"

namespace emulator {

uint8_t *Serial::sb = &Memory::registers[0xff01];
uint8_t *Serial::sc = &Memory::registers[0xff02];

uint8_t Serial::clk = 0;
uint8_t Serial::shifts = 0;

void Serial::tick()
{
    if (!check_bit(7, *sc)) // verify that transfer was requested
    {
        return;
    }

    if (check_bit(0, *sc)) // clock source is internal (8192Hz or 128 M-cycles)
    {
        if (++clk >= 32) // synchronize to 8192Hz
        {
            clk = 0;
            
            *sb = *sb << 1;
            shifts++;
            if (shifts >= 8)
            {
                shifts = 0;
                reset_bit(7, *sc);

                CPU::interrupt(SERIAL_INT);
            }
        }
    }
    else // clock source is external (up to 500kHz)
    {
        // needed?
    }
}
    
} // namespace emulator