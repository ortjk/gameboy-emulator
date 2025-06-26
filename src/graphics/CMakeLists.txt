set(SOURCE_LIST window.cpp 
                gpu.cpp)

set(HEADER_LIST "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/graphics/window.hpp"
                "${GameboyEmulator_SOURCE_DIR}/include/gameboy-emulator/graphics/gpu.hpp")

add_library(graphics_library "${SOURCE_LIST}" "${HEADER_LIST}")
target_link_libraries(graphics_library PRIVATE core_library OpenGL GLEW glfw)
target_include_directories(graphics_library PUBLIC "${GameboyEmulator_SOURCE_DIR}/include")