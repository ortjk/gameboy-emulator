#pragma once

#include "gameboy-emulator/graphics/sprite.hpp"

#include <cstdint>
#include <memory>

namespace emulator {

class Screen {
private:
    static const char *vertex_s;
    static const char *fragment_s;

    static const float vertices[5 * 4];
    static const unsigned int indices[3 * 2];

    static std::unique_ptr<Sprite> screen_sprite;
    static unsigned int VAO;
    static unsigned int program;
public:
    static uint8_t pixels[160 * 144];

    static void init();

    static void render();

};

} // namespace emulator
