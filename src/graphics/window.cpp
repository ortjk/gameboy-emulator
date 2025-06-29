#include "gameboy-emulator/graphics/window.hpp"
#include "gameboy-emulator/graphics/gpu.hpp"

#include <math.h>
#include <iostream>

namespace emulator {

GLFWwindow *Window::window = nullptr;
uint8_t Window::pixels[160 * 144] = {};

void Window::init_window()
{
    glfwInit();
    window = glfwCreateWindow(160, 144, "emulator", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();
    glOrtho(0, 160, 0, 144, 1, -1);
    glViewport(80, 72, 160, 144);
}

void Window::render()
{
    double t0 = 1.;
    double t1 = glfwGetTime();
    double dt;
    double mc; // master clock
    double vc; // visuals clock
    while (!glfwWindowShouldClose(window))
    {
        t0 = glfwGetTime();
        dt = t0 - t1;
        t1 = t0;
        
        mc += dt;
        vc += dt;

        if (mc >= (1. / 4194304.))
        {
            mc = 0;

            Motherboard::read_rom();
        }

        if (vc >= (1. / 59.73))
        {
            vc = 0;

            glClearColor(0.00f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glBegin(GL_POINTS);
                GPU::render(pixels);

                for (int y = 0; y < 144; y++)
                {
                    for (int x = 0; x < 160; x++)
                    {
                        uint8_t color = pixels[x + (y * 160)];
                        glColor3f(color / 64.f, color / 64.f, 0);
                        glVertex3f((float)x, (float)y, 0.0f);
                    }
                }
            glEnd();

            glfwSwapBuffers(window);
        }

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

    render();

    close();
}

} // namespace emulator
