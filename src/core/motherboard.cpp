#include "gameboy-emulator/core/motherboard.hpp"

#include "gameboy-emulator/core/cpu.hpp"
#include "gameboy-emulator/core/memory.hpp"
#include "gameboy-emulator/graphics/gpu.hpp"
#include "gameboy-emulator/peripherals/timer.hpp"
#include "gameboy-emulator/peripherals/joypad.hpp"
#include "gameboy-emulator/peripherals/serial.hpp"

#include <iostream>
#include <fstream>

namespace emulator {

uint8_t Motherboard::line = 0;
uint8_t Motherboard::w = 0;

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

void Motherboard::close_rom()
{

}

void Motherboard::gpu_process(uint8_t *pixels, uint32_t &dots_delta, uint16_t &gpu_delay)
{
    if (line < 144)
    {
        uint32_t dots = dots_delta % 456;
        if (dots == 0) // oam scan
        {
            GPU::scan_oam(line);
            dots_delta += 80;
            gpu_delay = 76;
        }
        else if (dots < 240) // drawing pixels
        {
            GPU::advance_fifo(pixels, w, line);
            w += 8;
            dots_delta += 8;
            gpu_delay += 4;
        }
        else // horizontal blank
        {
            GPU::hblank(line);
            line += 1;
            w = 0;

            dots_delta += 216;
            gpu_delay += 212;
        }
    }
    else  // vertical blank
    {
        GPU::vblank(line);
        dots_delta += 456;
        gpu_delay += 452;
        line += 1;

        if (line == 154)
        {
            line = 0;
        }
    }
}

void Motherboard::peripheral_tick(const uint8_t &buttons, const bool &joypad_int)
{
    Joypad::tick(buttons, joypad_int);
    Serial::tick();
    Timer::tick();
}

} // namespace emulator
