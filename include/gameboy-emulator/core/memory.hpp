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

public:
    static uint8_t registers[65536];
    static uint8_t dmg[256];

    /**
     *@brief Interpret 2 bytes starting at address as a 16-bit number
     *
     *@param address address of first byte
     */
    static uint16_t *get_16b(const uint16_t &address);

    /**
     *@brief Get pointer to memory explicitly from the dmg array (ignore 0xFF50).
     *
     *@param address Address of byte to access
     *
     *@returns Byte in _dmg at address.
     */
    static uint8_t *get_8b_dmg(const uint8_t &address);

    /**
     *@brief Swap dmg portion of ROM (0x00-0xFF) with stored dmg variable
     */
    static void unmap_dmg();

#ifdef CMAKE_BUILD_TESTING
    static void write(const uint8_t &b, const uint16_t &address);
    static void write(const uint16_t &b, const uint16_t &address);
    static void write(const uint8_t &msb, const uint8_t &lsb, const uint16_t &address);
#endif
};

} // namespace emulator

