#pragma once

#include <cstdint>

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

    // object attribute memory (oam) stored from 0xfe00-fe9f
    // contains 40 objects of 4 bytes each (160 bytes)
    // further information found here https://gbdev.io/pandocs/OAM.html
    static const uint8_t *oam;

    static const uint8_t *lcdc;
    static const uint8_t *scry;
    static const uint8_t *scrx;
    static const uint8_t *lyc;
    static const uint8_t *bgp;
    static const uint8_t *obp;
    static const uint8_t *wy;
    static const uint8_t *wx;

    /* MODIFIABLE REGISTERS */
    // interrupt flags
    static uint8_t *_if;
    // gpu status
    static uint8_t *stat;
    // current line
    static uint8_t *ly; 

    static uint8_t *fifo;

    static uint8_t num_objs;
    static uint8_t objs[10];

    static uint8_t wy_i; // internal window line counter

    /**
     *@brief Handles setting the render mode
     *
     *@param mode integer representing the new mode to set
     */
    static void set_mode(const uint8_t &mode);

    /**
     *@brief Maps a tile index to the corresponding position in VRAM
     *
     * Uses lcdc bit 4 to choose between maps
     *
     *@param index tile index
     */
    static const uint8_t *get_tile_data(const uint8_t &index);

    /**
     *@brief Get the pixel based on the screen position of a tile
     * 
     * tile values are broken down into an 8x8 grid of colors.
     * tile data is stored in memory in a unique way; two bytes of data (b0 and b1) correspond
     * to a byte containing the least significant bits (b0) and most significant bits (b1) in a row.
     * e.g. for tile data b0,b1 = 0x3c,0x73 = 0b0011_1100,0b0111_1110 row is
     *   0x  0x  1x  1x  1x  1x  0x  0x
     * + x0  x1  x1  x1  x1  x1  x1  x0
     *  ----------------------------------
     *   00  01  11  11  11  11  01  00
     */
    static uint8_t get_tile_pixel(const uint8_t *tile, const uint8_t &x, const uint8_t &y);

    /**
     *@brief Push the next 8 background pixels to the pixel FIFO
     *
     * Described (partially) by https://gbdev.io/pandocs/Tile_Maps.html and https://gbdev.io/pandocs/Scrolling.html
     *
     *@param x_start the starting horizontal position of the next 8 pixels
     *@param y the vertical position of the next 8 pixels
     */
    static void push_background_pixels(const uint16_t &x_start, const uint16_t &y);

    /**
     *@brief Push the next 8 window pixels to the pixel FIFO
     *
     * Described (partially) by https://gbdev.io/pandocs/Tile_Maps.html and https://gbdev.io/pandocs/Scrolling.html
     *
     *@param x_start the starting horizontal position of the next 8 pixels
     *@param y the vertical position of the next 8 pixels
     */
    static void push_window_pixels(const uint16_t &x_start, const uint16_t &y);

    /**
     *@brief Push the next 8 object pixels to the pixel FIFO
     *
     * Described (partially) by https://gbdev.io/pandocs/OAM.html
     *
     *@param x_start the starting horizontal position of the next 8 pixels
     *@param y the vertical position of the next 8 pixels
     */
    static void push_object_pixels(const uint16_t &x_start, const uint16_t &y);
public:
    /**
     *@brief Determine which objects will be present on the current line
     *
     * Adds 10 objects which intersect y to objs array. Called at the start of every line
     *
     *@param y current rendering line
     */
    static void scan_oam(const uint8_t &y);

    /**
     *@brief Master function for calling the pixel push functions
     *
     * Assigns pixel FIFO to the current position in the pixel buffer. Takes into account relevant lcdc bits
     *
     *@param pixels pixel buffer to be updated
     *@param x the starting horizontal position of the next 8 pixels
     *@param y the vertical position of the next 8 pixels
     */
    static void advance_fifo(uint8_t *pixels, const uint16_t &x, const uint16_t &y);

    /**
     *@brief Handles updating variables relevant to the hblank part of the rendering process
     *
     *@param y current rendering line
     */
    static void hblank(const uint16_t &y);

    /**
     *@brief Handles updating variables relevant to the vblank part of the rendering process
     *
     *@param y current rendering line
     */
    static void vblank(const uint8_t &y);
};

} // namespace emulator
