#pragma once

#include <cstdint>

namespace emulator {

class Joypad {
private:
    // joypad register
    static uint8_t *joyp;
    // interrupt flags
    static uint8_t *_if;
public:
    /**
     *@brief Update joypad registers
     */
    static void tick(const uint8_t &buttons, const bool &joypad_int);
};

} // namepace emulator
