#pragma once

#include <cstdint>

namespace emulator {

class OAMDMA {
private:
    // OAM DMA source address & start (range 0x00 to 0xDF)
    static const uint8_t *dma;

    // last dma code 
    // static uint8_t cache;
    // words transfered
    static uint8_t count;
    // in-progress transfer?
    static bool transferring;
public:
    static void tick();

};

} // namespace emulator
