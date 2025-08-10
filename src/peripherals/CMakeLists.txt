set(SOURCE_LIST timer.cpp
                joypad.cpp
                serial.cpp)

set(HEADER_LIST "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/peripherals/timer.hpp"
                "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/peripherals/joypad.hpp"
                "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/peripherals/serial.hpp")

add_library(peripherals_library "${SOURCE_LIST}" "${HEADER_LIST}")
target_link_libraries(peripherals_library PRIVATE core_library)
target_include_directories(peripherals_library PUBLIC "${GameboyEmulator_SOURCE_DIR}/include")