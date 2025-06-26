#include "gameboy-emulator/graphics/window.hpp"
#include "gameboy-emulator/graphics/gpu.hpp"

#include <math.h>
#include <iostream>

namespace emulator {

GLFWwindow *Window::window = nullptr;
uint8_t Window::pixels[144 * 160] = {};

void Window::init_window()
{
    glfwInit();
    window = glfwCreateWindow(144, 160, "emulator", NULL, NULL);
    glfwMakeContextCurrent(window);

    glewInit();
    glOrtho(0, 144, 0, 160, 1, -1);
    glViewport(72, 80, 144, 160);
}

void Window::render()
{
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.00f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_POINTS);
            GPU::render(pixels);

            for (int y = 0; y < 160; y++)
            {
                for (int x = 0; x < 144; x++)
                {
                    uint8_t color = pixels[x + (y * 144)];
                    glColor3f(color / 64.f, color / 64.f, 0);
                    glVertex3f((float)x, (float)y, 0.0f);
                }
            }
        glEnd();

        glfwSwapBuffers(window);

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
