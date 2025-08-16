#pragma once

#include <cstdint>

namespace emulator {

class Screen {
private:
    static const char *vertex_s;
    static const char *fragment_s;

    static const float vertices[5 * 4];
    static const unsigned int indices[3 * 2];

    static unsigned int texture;
    static unsigned int VAO;
    static unsigned int program;

    static void update_texture();
public:
    static uint8_t pixels[160 * 144];

    static void init();

    static void render();

};

} // namespace emulator
