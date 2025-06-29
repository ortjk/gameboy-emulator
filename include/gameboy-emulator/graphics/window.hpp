#pragma once

#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gameboy-emulator/graphics/gpu.hpp"
#include "gameboy-emulator/core/motherboard.hpp"

namespace emulator {

class Window {
private:
    static GLFWwindow *window;
    static uint8_t pixels[160 * 144];

    static void init_window();

    static void render();
    static void close();
public:
    static void open();
};

} // namespace emulator
