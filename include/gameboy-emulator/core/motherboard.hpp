#pragma once

#include <cstdint>

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
