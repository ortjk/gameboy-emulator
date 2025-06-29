#include "gameboy-emulator/graphics/gpu.hpp"

#include "gameboy-emulator/core/memory.hpp"

namespace emulator {

const uint8_t *GPU::tile_data0 = &Memory::registers()[0x8000];
const uint8_t *GPU::tile_data1 = &Memory::registers()[0x8800];
const uint8_t *GPU::tile_data2 = &Memory::registers()[0x9000];

const uint8_t *GPU::tile_map0 = &Memory::registers()[0x9800];
const uint8_t *GPU::tile_map1 = &Memory::registers()[0x9c00];
const uint8_t *GPU::oam = &Memory::registers()[0xfe00];

uint8_t GPU::scrx = 0;
uint8_t GPU::scry = 0;


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

    lsb = (lsb >> (7 - (x % 7))) & 1; // isolate ls column bit
    msb = ((msb >> (7 - (x % 7))) & 1) << 1; // isolate ms column bit << 1

    return lsb | msb; // return 2-bit color index (pixel)
}

void GPU::render_background_line(uint8_t* pixels, const int &y)
{
    for (int x = 0; x < 160; x++)
    {
        uint8_t tile = tile_map0[PIXELS_TO_TILE_MAP(x, y)];
        pixels[x + (y * 160)] = get_tile_pixel(&tile_data0[tile], x, y);
    }
}

void GPU::render_object_line(uint8_t* pixels, const int &y)
{

}

void GPU::render(uint8_t* pixels)
{
    uint8_t *ly = Memory::get_8b(0xFF44);
    for (int y = 0; y < 144; y++)
    {
        *ly = static_cast<uint8_t>(y);
        render_background_line(pixels, y);
    }
    *ly = 144;
}

} // namespace emulator
