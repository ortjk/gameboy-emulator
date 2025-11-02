#include "gameboy-emulator/graphics/oam_dma.hpp"

#include "gameboy-emulator/core/memory.hpp"
#include "gameboy-emulator/core/bytelib.hpp"

namespace emulator {

const uint8_t *OAMDMA::dma = &Memory::registers[0xff46];

uint8_t OAMDMA::count = 0;
bool OAMDMA::transferring = false;

void OAMDMA::tick()
{
    if (!transferring)
    {
        if (Memory::wrote && Memory::last_addr == 0xff46)
        {
            transferring = true;
            count = 0;
        }
        else 
        { 
            return; 
        }
    }
    
    for (int i = 0; i < 4; i++)
    {
        Memory::registers[0xfe00 + count] = Memory::registers[bytes_to_16b(*dma, count)];
        count++;
    }

    // after 160 transfers, stop
    if (count >= 0x9f)
    {
        transferring = false;
    }
}

} // namespace emulator
