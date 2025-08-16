set(SOURCE_LIST window.cpp 
                gui.cpp
                gpu.cpp
                screen.cpp)

set(HEADER_LIST "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/graphics/window.hpp"
                "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/graphics/gui.hpp"
                "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/graphics/gpu.hpp"
                "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/graphics/screen.hpp")

add_library(graphics_library "${SOURCE_LIST}" "${HEADER_LIST}")
target_link_libraries(graphics_library PRIVATE core_library OpenGL GLEW glfw imgui_library)
target_include_directories(graphics_library PUBLIC "${GameboyEmulator_SOURCE_DIR}/include")