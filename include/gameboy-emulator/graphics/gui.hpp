#pragma once

#include "gameboy-emulator/graphics/sprite.hpp"

#include "imgui/imgui.h"

#include <GLFW/glfw3.h>
#include <memory>

namespace emulator {

class GUI {
private:
    static ImGuiIO *io;
    static std::unique_ptr<Sprite> objects[40];

    static void memory_viewer();
    static void object_viewer();
public:
    static void init(GLFWwindow *window);
    static void draw();
    static void render();
    static void close();
};

} // namespace emulator
