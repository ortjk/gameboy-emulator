#include "gameboy-emulator/graphics/gpu.hpp"

#include "gameboy-emulator/core/cpu.hpp"
#include "gameboy-emulator/core/bytelib.hpp"
#include "gameboy-emulator/core/memory.hpp"

#include <iostream>

namespace emulator {

const uint8_t *GPU::tile_data0 = &Memory::registers[0x8000];
const uint8_t *GPU::tile_data1 = &Memory::registers[0x8800];
const uint8_t *GPU::tile_data2 = &Memory::registers[0x9000];

const uint8_t *GPU::tile_map0 = &Memory::registers[0x9800];
const uint8_t *GPU::tile_map1 = &Memory::registers[0x9c00];

const uint8_t *GPU::lcdc = &Memory::registers[0xff40];
const uint8_t *GPU::scry = &Memory::registers[0xff42];
const uint8_t *GPU::scrx = &Memory::registers[0xff43];
const uint8_t *GPU::lyc = &Memory::registers[0xff45];
const uint8_t *GPU::bgp = &Memory::registers[0xff47]; 
const uint8_t *GPU::wy = &Memory::registers[0xff4a];
const uint8_t *GPU::wx = &Memory::registers[0xff4b];

const uint8_t *GPU::oam = &Memory::registers[0xfe00];
const uint8_t *GPU::obp = &Memory::registers[0xff48];

uint8_t *GPU::stat = &Memory::registers[0xff41];
uint8_t *GPU::ly = &Memory::registers[0xff44];

uint8_t *GPU::fifo = nullptr;

uint8_t GPU::num_objs = 0;
uint8_t GPU::objs[10] = {};

uint8_t GPU::wy_i = 0;

void GPU::set_mode(const uint8_t &mode)
{
    *stat = (*stat & 0xFC) | mode; // set stat mode
    if (mode != 3 && check_bit(3 + mode, *stat))
    {
        CPU::interrupt(STAT_INT);
    }
}

const uint8_t *GPU::get_tile_data(const uint8_t &index)
{
    if (check_bit(4, *lcdc))
    {
        return tile_data0;
    }
    else
    {
        if (index < 128)
        {
            return tile_data2;
        }
        else 
        {
            return tile_data0;
        }
    }
}

// tile values are broken down into an 8x8 grid of colors.
// there are 4 possible colors, so 1 color is 2 bits: [0, 1, 2, 3] = [0%, 33%, 66%, 100%].
// which color is 0% and which is 100% is stored per-layer.
// 
// tile data is stored in memory in a unique way; two bytes of data (b0 and b1) correspond
// to a byte containing the least significant bits (b0) and most significant bits (b1) in a row.
// e.g. for tile data b0,b1 = 0x3c,0x73 = 0b0011_1100,0b0111_1110 row is
//   0x  0x  1x  1x  1x  1x  0x  0x
// + x0  x1  x1  x1  x1  x1  x1  x0
// ----------------------------------
//   00  01  11  11  11  11  01  00
//
uint8_t GPU::get_tile_pixel(const uint8_t *tile_data, const uint8_t &x, const uint8_t &y)
{
    uint8_t lsb = tile_data[(y % 8) * 2]; // get byte containing ls row data
    uint8_t msb = tile_data[(y % 8) * 2 + 1]; // get byte containing ms row data

    lsb = (lsb >> (7 - (x % 8))) & 1; // isolate ls column bit
    msb = (msb >> (7 - (x % 8))) & 1; // isolate ms column bit

    return lsb | (msb << 1); // return 2-bit color index (pixel)
}

void GPU::scan_oam(const uint8_t &y)
{
    // set current line register (ly)
    *ly = y;

    set_mode(2);
    if (*ly == *lyc && check_bit(6, *stat)) 
    { 
        set_bit(2, *stat); 
        CPU::interrupt(STAT_INT); 
    }

    // reset oam scan
    num_objs = 0;
    std::fill(std::begin(objs), std::end(objs), 0xFF);
    
    for (uint8_t i = 0; i < 40; i++)
    {
        const uint8_t obj_y = oam[i * 4];
        if (check_bit(2, *lcdc))
        {
            if (obj_y <= y + 16 && y + 16 < obj_y + 16)
            {
                objs[num_objs] = i;
                num_objs++;
                if (num_objs == 10)
                {
                    return;
                }
            }
        }
        else
        {
            if (obj_y <= y + 16 && y + 16 < obj_y + 8) // check if object has rows within current scanned row
            {
                objs[num_objs] = i;
                num_objs++;
                if (num_objs == 10)
                {
                    return;
                }
            }
        }
    }
}

void GPU::push_background_pixels(const uint16_t &x_start, const uint16_t &y)
{
    for (int i = 0; i < 8; i++)
    {
        uint8_t tile;
        if (check_bit(3, *lcdc)) // check BG tile map
        {
            tile = tile_map1[PIXELS_TO_BG_TILE_MAP(i + x_start, y, *scrx, *scry)]; // tile map is 0x9C00-0x9FFF
        }
        else 
        {
            tile = tile_map0[PIXELS_TO_BG_TILE_MAP(i + x_start, y, *scrx, *scry)]; // tile map is 0x9800-0x9BFF
        }

        const uint8_t *tile_data = get_tile_data(tile);
        uint8_t pixel = get_tile_pixel(&tile_data[tile * 16], i + x_start + *scrx, y + *scry);
        fifo[i] = COLOR_FROM_PALETTE(pixel, bgp);
    }
}

void GPU::push_window_pixels(const uint16_t &x_start, const uint16_t &y)
{
    if (*wy > y) // check row is in boundaries of window
    {
        return;
    }

    for (int i = 0; i < 8; i++)
    {
        if (*wx < x_start + i + 8) // check pixel will be in boundaries of window
        {
            uint8_t tile;
            if (check_bit(6, *lcdc)) // check window tile map
            {
                tile = tile_map1[PIXELS_TO_WIN_TILE_MAP(x_start + i + 7, wy_i, *wx)]; // tile map is 0x9C00-0x9FFF
            }
            else
            {
                tile = tile_map0[PIXELS_TO_WIN_TILE_MAP(x_start + i + 7, wy_i, *wx)]; // tile map is 0x9800-0x9BFF
            }

            const uint8_t *tile_data = get_tile_data(tile);
            uint8_t pixel = get_tile_pixel(&tile_data[tile * 16], i + x_start + 7 - *wx, wy_i);
            fifo[i] = COLOR_FROM_PALETTE(pixel, bgp);
        }
    }
}

void GPU::push_object_pixels(const uint16_t &x_start, const uint16_t &y)
{
    uint8_t x_prios[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    for (int i = 0; i < 8; i++)
    {
        for (int o = 0; o < num_objs; o++)
        {
            const uint8_t *obj = &oam[objs[o] * 4];
            if (obj[1] < x_start + i + 9 && obj[1] + 8 >= x_start + i + 9 && obj[1] < x_prios[i])
            {
                uint8_t tile = obj[2];
                if (check_bit(2, *lcdc)) // check for 8x16 mode
                {
                    if (tile % 2 == 1) // ignore bit 0 in 8x16 mode
                    {
                        tile -= 1;
                    }

                    if (y + 16 - obj[0] >= 8) // check if in bottom tile
                    {
                        if (!check_bit(6, obj[3]))
                        {
                            tile += 1;
                        }
                    }
                    else
                    {
                        if (check_bit(6, obj[3]))
                        {
                            tile += 1;
                        }
                    }
                }

                uint8_t _x = x_start + i - obj[1]; 
                uint8_t _y = y - obj[0];
                if (check_bit(5, obj[3])) { _x = 7 - (_x % 8); } // vertical mirror
                if (check_bit(6, obj[3])) { _y = 7 - (_y % 8); } // horizontal mirror
                
                uint8_t pixel = get_tile_pixel(&tile_data0[tile * 16], _x, _y);
                if (pixel > 0) // ignore transparent object pixels
                {
                    if (!(fifo[i] > 0 && check_bit(7, obj[3]))) // check whether background takes priority
                    {
                        fifo[i] = COLOR_FROM_PALETTE(pixel, (&obp[check_bit(4, obj[3])]));
                        x_prios[i] = obj[1];
                    }
                }
            }
        }
    }
}

void GPU::advance_fifo(uint8_t *pixels, const uint16_t &x, const uint16_t &y)
{
    if (x == 0) // check for beginning of drawing pixels
    { 
        set_mode(3); // set drawing pixels mode
    }

    fifo = &pixels[x + (y * 160)];

    if (check_bit(0, *lcdc)) // check bg & window enable
    {
        push_background_pixels(x, y);
        if (check_bit(5, *lcdc)) // check window enable
        {
            push_window_pixels(x, y);
        }
    }
    else
    {
        std::fill(&fifo[0], &fifo[7], 0); // draw all zeros to screen
    }

    if (check_bit(1, *lcdc)) // check obj enable
    {
        push_object_pixels(x, y);
    }
}

void GPU::hblank(const uint16_t &y)
{
    // increment internal window line counter
    if (*wx < 166 && *wy <= y // check window is on screen
        && check_bit(0, *lcdc) && check_bit(5, *lcdc)) // check window is enabled
    {
        wy_i += 1;
    }

    set_mode(0); // set horizontal blank mode
}

void GPU::vblank(const uint8_t &y)
{
    *ly = y;
    if (*ly == *lyc && check_bit(6, *stat))
    {
        set_bit(2, *stat); 
        CPU::interrupt(STAT_INT); 
    }
    if (y == 144)
    {
        wy_i = 0; // reset internal window line counter
        set_mode(1); // set vertical blank mode
        CPU::interrupt(VBLANK_INT);
    }
}

} // namespace emulator
