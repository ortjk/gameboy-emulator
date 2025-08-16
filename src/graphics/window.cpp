#include "gameboy-emulator/graphics/window.hpp"

#include "gameboy-emulator/core/motherboard.hpp"
#include "gameboy-emulator/core/bytelib.hpp"
#include "gameboy-emulator/graphics/screen.hpp"

#include <GLFW/glfw3.h>
#include <math.h>
#include <chrono>
#include <thread>

namespace emulator {

GLFWwindow *Window::window = nullptr;
const GLFWvidmode *Window::mode = nullptr;
bool Window::key_update = false;

void Window::init_window()
{
    glfwInit();
    window = glfwCreateWindow(160, 144, "emulator", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, on_key_press);

    glewInit();
    glOrtho(0, 160, 144, 0, 1, -1);
    glViewport(80, 72, 160, 144);
}

void Window::on_key_press(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_RELEASE)
    {
        key_update = true;
    }
}

void Window::update_keys(uint8_t &buttons, bool &joypad_int)
{
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        reset_bit(7, buttons);
        joypad_int = true;
    }
    else 
    {
        set_bit(7, buttons);
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
    {
        reset_bit(6, buttons);
        joypad_int = true;
    }
    else 
    {
        set_bit(6, buttons);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        reset_bit(5, buttons);
        joypad_int = true;
    }
    else 
    {
        set_bit(5, buttons);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        reset_bit(4, buttons);
        joypad_int = true;
    }
    else 
    {
        set_bit(4, buttons);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        reset_bit(3, buttons);
        joypad_int = true;
    }
    else 
    {
        set_bit(3, buttons);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        reset_bit(2, buttons);
        joypad_int = true;
    }
    else 
    {
        set_bit(2, buttons);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        reset_bit(1, buttons);
        joypad_int = true;
    }
    else 
    {
        set_bit(1, buttons);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        reset_bit(0, buttons);
        joypad_int = true;
    }
    else 
    {
        set_bit(0, buttons);
    }
}

void Window::blit()
{
    glClearColor(0.00f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Screen::render();

    glfwSwapBuffers(window);
}

void Window::game_loop()
{
    const double refreshPeriod = 1. / 60.; // mode->refreshRate;

    const uint32_t cycles_per_frame = 70224;
    uint32_t cycles_delta = 0; // cpu cycles since frame start
    
    uint32_t dots_delta = 0; // position in render cycle
    uint16_t gpu_delay = 0; // delay resulting from gpu operation

    uint8_t line = 0; // current line in display
    uint8_t w = 0; // pixel fifo position

    uint8_t buttons = 0xFF; // byte that contains the input keys
    while (!glfwWindowShouldClose(window))
    {
        double frame_start = glfwGetTime();

        while (cycles_delta < cycles_per_frame)
        {
            Motherboard::read_rom(cycles_delta);

            bool joypad_int = false; // only trigger joypad interrupts on key press
            if (key_update)
            {
                update_keys(buttons, joypad_int);
                key_update = false;
            }

            Motherboard::peripheral_tick(buttons, joypad_int);

            if (gpu_delay == 0)
            {
                Motherboard::gpu_process(Screen::pixels, dots_delta, gpu_delay);
            }
            else 
            {
                gpu_delay -= 4;
            }

            Motherboard::handle_interrupts();
        }

        blit();

        cycles_delta = 0;
        dots_delta = 0;
        
        int time_to_next = (int)((refreshPeriod - glfwGetTime() + frame_start) * 1000.);
        std::this_thread::sleep_for(std::chrono::milliseconds(time_to_next));


        glfwPollEvents();
    }
}

void Window::close()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::open()
{
    init_window();

    game_loop();

    close();
}

} // namespace emulator
