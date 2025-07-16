#include "gameboy-emulator/graphics/window.hpp"
#include "gameboy-emulator/graphics/gpu.hpp"

#include <GLFW/glfw3.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <iostream>

namespace emulator {

GLFWwindow *Window::window = nullptr;
const GLFWvidmode *Window::mode = nullptr;
uint8_t Window::pixels[160 * 144] = {};

void Window::init_window()
{
    glfwInit();
    // GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    window = glfwCreateWindow(160, 144, "emulator", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();
    glOrtho(0, 160, 144, 0, 1, -1);
    glViewport(80, 72, 160, 144);

    // mode = glfwGetVideoMode(monitor);
}

void Window::blit()
{
    glClearColor(0.00f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_POINTS);
        for (int y = 0; y < 144; y++)
        {
            for (int x = 0; x < 160; x++)
            {
                uint8_t color = pixels[x + (y * 160)];
                glColor3f(0.f, color / 3.f, 0.f);
                glVertex3f((float)x, (float)y, 0.0f);
            }
        }
    glEnd();

    glfwSwapBuffers(window);
}

void Window::game_loop()
{
    const double refreshPeriod = 1. / 60.; // mode->refreshRate;

    const uint32_t cycles_per_frame = 70224;
    uint32_t cycles_delta = 0;
    
    uint32_t dots_delta = 0;
    uint16_t gpu_delay = 0;

    uint8_t line = 0;
    uint8_t w = 0; // pixel fifo position
    while (!glfwWindowShouldClose(window))
    {
        double frame_start = glfwGetTime();

        while (cycles_delta < cycles_per_frame)
        {
            Motherboard::read_rom(cycles_delta);

            if (gpu_delay == 0)
            {
                if (line < 144)
                {
                    uint32_t dots = dots_delta % 456;
                    if (dots == 0) // oam scan
                    {
                        GPU::scan_oam(line);
                        dots_delta += 80;
                        gpu_delay = 76;
                    }
                    else if (dots < 240) // drawing pixels
                    {
                        GPU::advance_fifo(pixels, w, line);
                        w += 8;
                        dots_delta += 8;
                        gpu_delay += 4;
                    }
                    else // horizontal blank
                    {
                        GPU::hblank(line);
                        line += 1;
                        w = 0;

                        dots_delta += 216;
                        gpu_delay += 212;
                    }
                }
                else  // vertical blank
                {
                    GPU::vblank(line);
                    dots_delta += 456;
                    gpu_delay += 452;
                    line += 1;
                }
            }
            else 
            {
                gpu_delay -= 4;
            }
        }

        blit();

        cycles_delta = 0;
        dots_delta = 0;
        line = 0;
        
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
