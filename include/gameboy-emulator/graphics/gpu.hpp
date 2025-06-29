#pragma once

#include <cstdint>

#include "gameboy-emulator/core/cpu.hpp"

#define PIXELS_TO_TILE_MAP(x, y) ((x / 8) + ((y / 8) * 32))

namespace emulator {

class GPU {
private:
    // tile data is stored in 3 blocks from 0x8000-0x97ff
    // there are two addressing modes, determined via LCDC bit 4

    // tile map stored from 0x8000-0x87ff (2048 bytes)
    static const uint8_t *tile_data0;
    // tile map stored from 0x8800-0x8fff (2048 bytes)
    static const uint8_t *tile_data1;
    // tile map stored from 0x9000-0x97ff (2048 bytes)
    static const uint8_t *tile_data2;

    // tile map stored from 0x9800-0x9bff (1024 bytes)
    static const uint8_t *tile_map0;
    // tile map stored from 0x9c00-0x9fff (1024 bytes)
    static const uint8_t *tile_map1;

    // object attribute memory (oam) stored from 0xfe00-fe9f
    // contains 40 objects of 4 bytes each (160 bytes)
    // further information found here https://gbdev.io/pandocs/OAM.html
    static const uint8_t *oam;

    static uint8_t scrx;
    static uint8_t scry;

    static uint8_t get_tile_pixel(const uint8_t *tile, const uint8_t &x, const uint8_t &y);

public:
    static void render_background_line(uint8_t* pixels, const int &y);
    static void render_object_line(uint8_t* pixels, const int &y);

    // gameboy renders row-by-row, left to right, @60hz
    static void render(uint8_t* pixels);
};

} // namespace emulator
