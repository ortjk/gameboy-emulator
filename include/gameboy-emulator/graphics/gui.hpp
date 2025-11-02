#pragma once

#include "gameboy-emulator/graphics/sprite.hpp"

#include "imgui/imgui.h"

#include <GLFW/glfw3.h>
#include <memory>

namespace emulator {

class GUI {
private:
    static const char *opcodes[256];

    static ImGuiIO *io;
    static std::unique_ptr<Sprite> objects[40];
    static char breakpoint_temp[5];

    static void memory_viewer();
    static void object_viewer();
    static void program_editor();
public:
    static bool paused;
    static bool step;
    static bool breakpoints[65536];

    static void init(GLFWwindow *window);
    static void draw();
    static void render();
    static void close();
};

} // namespace emulator
