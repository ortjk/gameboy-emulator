#pragma once

#include <cstdint>

#include "gameboy-emulator/core/cpu.hpp"

namespace emulator {

class Motherboard {
private:
    static uint8_t *pm; // program memory
    
    static void bootrom_emu();
public:
    static void read_rom();
    static void load_rom(const char *boot_path, const char *rom_path);
    static void load_rom(const char *rom_path);
    static void close_rom();


};

} // namespace emulator
