set(SOURCE_LIST cpu.cpp
                memory.cpp
                instructions.cpp
                alu.cpp
                bytelib.cpp)

set(HEADER_LIST "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/core/cpu.hpp" 
                "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/core/memory.hpp" 
                "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/core/instructions.hpp"
                "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/core/alu.hpp"
                "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/core/bytelib.hpp")

add_library(core_library "${SOURCE_LIST}" "${HEADER_LIST}")
target_include_directories(core_library PUBLIC "${GameboyEmulator_SOURCE_DIR}/include")
