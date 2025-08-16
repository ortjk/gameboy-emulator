#include "gameboy-emulator/graphics/screen.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace emulator {

uint8_t Screen::pixels[160 * 144] = {};

void Screen::render()
{
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
}

} // namespace emulator
