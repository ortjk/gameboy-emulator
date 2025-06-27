#include <iostream>
#include <string>

#include "gameboy-emulator/core/motherboard.hpp"

int main(int argc, char* argv[])
{
    const char *rom = NULL;

    for (int i = 0; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg.compare("-r") == 0 && i < argc - 1)
        {
            rom = argv[i + 1];
        }
    }

    if (rom)
    {
        emulator::Motherboard::load_rom(rom);
    }
    else 
    {
        std::cout << "no rom provided" << std::endl;
    }
}
