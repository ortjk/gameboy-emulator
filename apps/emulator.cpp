#include <iostream>
#include <string>

#include "gameboy-emulator/core/motherboard.hpp"
#include "gameboy-emulator/graphics/window.hpp"

int main(int argc, char* argv[])
{
    const char *boot = NULL;
    const char *rom = NULL;

    for (int i = 0; i < argc; i++)
    {
        std::string arg = argv[i];

        if (arg.compare("-b") == 0 && i < argc - 1)
        {
            boot = argv[i + 1];
        }
        if (arg.compare("-r") == 0 && i < argc - 1)
        {
            rom = argv[i + 1];
        }
    }

    if (rom)
    {
        if (boot)
        {
            emulator::Motherboard::load_rom(boot, rom);
        }
        else 
        {
            emulator::Motherboard::load_rom(rom);
        }
        emulator::Window::open();

        emulator::Motherboard::close_rom();
    }
    else 
    {
        std::cout << "no rom provided" << std::endl;
    }
}
