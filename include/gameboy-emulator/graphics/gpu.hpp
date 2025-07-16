#pragma once

#include <cstdint>

#include "gameboy-emulator/core/cpu.hpp"

#define PIXELS_TO_BG_TILE_MAP(x, y, scrx, scry) ( \
    (((x + scrx) % 256) / 8) + ((((y + scry) % 256) / 8) * 32) \
)

#define PIXELS_TO_WIN_TILE_MAP(x, wy_i, wx) ( \
    ((x - wx) / 8) + ((wy_i / 8) * 32) \
)

#define COLOR_FROM_PALETTE(i, pal) ( \
    (pal[0] >> (i * 2)) & 0b11 \
)

namespace emulator {

class GPU {
private:
    /* READONLY REGISTERS */
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

    static const uint8_t *lcdc;
    static const uint8_t *scry;
    static const uint8_t *scrx;
    static const uint8_t *lyc;
    static const uint8_t *bgp;
    static const uint8_t *wy;
    static const uint8_t *wx;

    // object attribute memory (oam) stored from 0xfe00-fe9f
    // contains 40 objects of 4 bytes each (160 bytes)
    // further information found here https://gbdev.io/pandocs/OAM.html
    static const uint8_t *oam;
    static const uint8_t *obp;

    /* MODIFIABLE REGISTERS */
    // gpu status
    static uint8_t *stat;
    // current line
    static uint8_t *ly; 

    static uint8_t *fifo;

    static uint8_t num_objs;
    static uint8_t objs[10];

    static uint8_t wy_i; // internal window line counter

    static void set_mode(const uint8_t &mode);
    static const uint8_t *get_tile_data(const uint8_t &index);
    static uint8_t get_tile_pixel(const uint8_t *tile, const uint8_t &x, const uint8_t &y);

    static void push_background_pixels(const uint16_t &x_start, const uint16_t &y);
    static void push_window_pixels(const uint16_t &x_start, const uint16_t &y);
    static void push_object_pixels(const uint16_t &x_start, const uint16_t &y);
public:
    static void scan_oam(const uint8_t &y);
    static void advance_fifo(uint8_t *pixels, const uint16_t &x, const uint16_t &y);
    static void hblank(const uint16_t &y);
    static void vblank(const uint8_t &y);
};

} // namespace emulator
