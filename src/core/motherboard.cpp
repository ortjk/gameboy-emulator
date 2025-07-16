#include "gameboy-emulator/core/motherboard.hpp"

#include <iostream>
#include <fstream>

namespace emulator {

void Motherboard::bootrom_emu()
{
    CPU::instruction(0x31, 0xfe, 0xff, 0x00); // load sp
    CPU::pc = 0x0100; // set pc
}

void Motherboard::read_rom(uint32_t &cycles_delta)
{
    if (CPU::t == 0)
    {
        uint8_t *instr = &Memory::registers[CPU::pc];
        CPU::instruction(instr[0], instr[1], instr[2], instr[3]);
        cycles_delta += CPU::t;
    }

    CPU::t -= 4;
}

void Motherboard::load_rom(const char *boot_path, const char *rom_path)
{
    // open binary file input streams with position at eof
    std::ifstream boot(boot_path, std::ios::in|std::ios::binary|std::ios::ate);
    std::ifstream rom(rom_path, std::ios::in|std::ios::binary|std::ios::ate);
    if (boot.is_open() && rom.is_open())
    {
        uint8_t *pm = &Memory::registers[0x0000];
        uint8_t *dmg = &Memory::dmg[0x00];
        
        std::streampos boot_end = boot.tellg();
        std::streampos rom_end = rom.tellg();

        boot.seekg(0, std::ios::beg);
        boot.read(reinterpret_cast<char *>(&pm[0]), boot_end);

        rom.seekg(0, std::ios::beg);
        rom.read(reinterpret_cast<char *>(&dmg[0]), 256);
        rom.seekg(256, std::ios::beg);
        rom.read(reinterpret_cast<char *>(&pm[256]), rom_end);

        boot.close();
        rom.close();
    }
    else 
    {
        std::cout << "error opening boot or rom at paths: " << boot_path << ", " << rom_path << std::endl;
    }
}
/*
void Motherboard::load_rom(const char *rom_path)
{
    // open binary file input stream with position at eof
    std::ifstream rom(rom_path, std::ios::in|std::ios::binary|std::ios::ate);
    if (rom.is_open())
    {
        std::streampos rom_size = rom.tellg();
        pm = new uint8_t[rom_size];

        rom.seekg(0, std::ios::beg);
        rom.read(reinterpret_cast<char *>(&pm[0]), rom_size);

        rom.close();

        bootrom_emu();
    }
    else 
    {
        std::cout << "error opening rom at path: " << rom_path << std::endl;
    }
}
*/

void Motherboard::close_rom()
{

}

} // namespace emulator
