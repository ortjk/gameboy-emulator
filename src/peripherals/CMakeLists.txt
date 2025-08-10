set(SOURCE_LIST timer.cpp)

set(HEADER_LIST "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/peripherals/timer.hpp")

add_library(peripherals_library "${SOURCE_LIST}" "${HEADER_LIST}")
target_link_libraries(peripherals_library PRIVATE core_library)
target_include_directories(peripherals_library PUBLIC "${GameboyEmulator_SOURCE_DIR}/include")