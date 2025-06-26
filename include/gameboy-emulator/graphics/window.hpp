#pragma once

#include <cstdint>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gameboy-emulator/graphics/gpu.hpp"

namespace emulator {

class Window {
private:
    static GLFWwindow *window;
    static uint8_t pixels[144 * 160];

    static void init_window();

    static void render();
    static void close();
public:
    static void open();
};

} // namespace emulator
