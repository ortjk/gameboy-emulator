#pragma once

#include <cstdint>

namespace emulator {

class Serial {
private:
    // serial transfer data
    static uint8_t *sb;
    // serial transfer control
    static uint8_t *sc;
    // interrupt flags
    static uint8_t *_if;

    static uint8_t clk;
    static uint8_t shifts;
public:
    static void tick();
};

} // namespace emulator