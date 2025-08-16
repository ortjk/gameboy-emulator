#pragma once

#include "imgui/imgui.h"

#include <GLFW/glfw3.h>

namespace emulator {

class GUI {
private:
    static ImGuiIO *io;

    static void memory_viewer();
    static void object_viewer();
public:
    static void init(GLFWwindow *window);
    static void draw();
    static void render();
    static void close();
};

} // namespace emulator
