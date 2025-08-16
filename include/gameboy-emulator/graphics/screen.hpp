#pragma once

#include <cstdint>

namespace emulator {

class Screen {
private:



public:
    static uint8_t pixels[160 * 144];

    static void render();

};

} // namespace emulator
