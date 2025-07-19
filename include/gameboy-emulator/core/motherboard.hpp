#pragma once

#include <cstdint>

namespace emulator {

class Motherboard {
private:
    static uint8_t line; // current line in display
    static uint8_t w; // pixel fifo position
public:
    /**
     *@brief Send an instruction to the CPU from the open rom
     *
     * Ensures a delay of 4 t-cycles.
     *
     *@param cycles_delta CPU cycles since frame start
     */
    static void read_rom(uint32_t &cycles_delta);

    /**
     *@brief Load the bootrom and rom into memory
     *
     * Opens a stream to the passed paths, and loads bootrom from 0x00 to 0xFF, and rom from 0x100 to end.
     * Caches rom from 0x00 to 0xFF for later use as well.
     *
     *@param boot_path path to bootrom
     *@param rom_path path to rom
     */
    static void load_rom(const char *boot_path, const char *rom_path);
    
    /**
     *@brief Closes the open rom
     *
     * Can be used to handle memory de-allocation
     */
    static void close_rom();

    /**
     *@brief Command the GPU to process a portion of existing VRAM
     *
     * Essentially a logic tree to call the proper GPU functions based on the current position in rendering process.
     *
     *@param pixels pixel buffer which is to be changed by GPU
     *@param dots_delta position in render cycle
     *@param gpu_delay delay resulting from gpu operation
     */
    static void gpu_process(uint8_t *pixels, uint32_t &dots_delta, uint16_t &gpu_delay);
};

} // namespace emulator
