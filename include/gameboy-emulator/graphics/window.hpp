#pragma once

#include <cstdint>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace emulator {

class Window {
private:
    static GLFWwindow *window;
    static const GLFWvidmode *mode;
    static uint8_t pixels[160 * 144];

    /**
     *@brief Create and configure the main emulator window
     *
     * Calls glfw functions to create window and context, and configures OpenGL viewport/orthographic view.
     */
    static void init_window();

    /**
     *@brief Clear the existing window and draw pixels from buffer
     */
    static void blit();

    /**
     *@brief Main loop for the emulator
     *
     * Every ~16.67s, emulates the number of cpu and gpu cycles that would have occured during that time frame.
     */
    static void game_loop();

    /**
     *@brief Close the glfw window
     */
    static void close();
public:
    /**
     *@brief Calls init_window(), then game_loop(), then close().
     */
    static void open();
};

} // namespace emulator
