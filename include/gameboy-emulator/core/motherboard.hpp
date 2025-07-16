#pragma once

#include <cstdint>

#include "gameboy-emulator/core/cpu.hpp"
#include "gameboy-emulator/core/memory.hpp"

namespace emulator {

class Motherboard {
private:
    static void bootrom_emu();
public:
    static void read_rom(uint32_t &cycles_delta);
    static void load_rom(const char *boot_path, const char *rom_path);
    static void load_rom(const char *rom_path);
    static void close_rom();


};

} // namespace emulator
