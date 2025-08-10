#pragma once

#include <cstdint>

namespace emulator {

class Timer {
private:
    /* READONLY REGISTERS */
    // timer modulo
    static const uint8_t *tma;
    // timer control
    static const uint8_t *tac;

    /* MODIFIABLE REGISTERS */
    // divider register
    static uint8_t *div;
    // timer counter
    static uint8_t *tima;

    static const uint8_t clk_sel[4];

    static uint8_t div_d;
    static uint8_t tima_d;

    /**
     *@brief Increment the divider register
     *
     * Constantly increment by 1 every 4 M-cycles, 
     * always enabled
     */
    static void increment_div();

    /**
     *@brief Increment the tima register
     *
     * Incrementation characteristics determined by tac register
     */
    static void increment_tima();
public:
    /**
     *@brief Updated timer registers
     */
    static void tick();
};

} // namespace emulator
