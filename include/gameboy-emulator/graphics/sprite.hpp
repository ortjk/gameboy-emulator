#pragma once

#include <GL/glew.h>
#include <cstdint>

namespace emulator {

class Sprite {
private:
    unsigned int width = 0;
    unsigned int height = 0;
    GLuint texture = 0;

public:
    Sprite(unsigned int x, unsigned int y);
    virtual ~Sprite();

    void update(const uint8_t pixels[], bool flip = false);
    const GLuint &get_texture();
};

} // namespace emulator
