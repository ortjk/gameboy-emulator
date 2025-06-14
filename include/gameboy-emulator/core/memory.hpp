#pragma once

#include <cstdint>

namespace emulator
{

class Memory
{
private:
    // gameboy memory map:
    // start    end      description
    // ---------------------------------------------
    // 0000     3FFF     16 KiB ROM bank 00
    // 4000     7FFF     16 KiB ROM bank 01-NN
    // 8000     9FFF     8 KiB VRAM
    // A000     BFFF     8 KiB External RAM
    // C000     CFFF     4 KiB WRAM
    // D000     DFFF     4 KiB WRAM
    // E000     FDFF     Mirror of C000-DDFF
    // FE00     FE9F     Object attribute memory
    // FEA0     FEFF     Not usable
    // FF00     FF7F     I/O registers
    // FF80     FFFE     High RAM
    // FFFF     FFFF     Interrupt enable register

    static uint8_t registers[65536];

public:
    static uint8_t *get_8b(const uint16_t &address);
    static uint16_t *get_16b(const uint16_t &address);

#ifdef CMAKE_BUILD_TESTING
    static void write(const uint8_t &b, const uint16_t &address);
    static void write(const uint16_t &b, const uint16_t &address);
    static void write(const uint8_t &msb, const uint8_t &lsb, const uint16_t &address);
#endif
};

} // namespace emulator

