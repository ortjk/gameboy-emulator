#include "gameboy-emulator/graphics/sprite.hpp"

namespace emulator {

Sprite::Sprite(unsigned int x, unsigned int y)
    : width(x), height(y)
{
    // create a texture identifier
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    // filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // upload blank as texture
    uint8_t pixels_rgb[x * y * 4];
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_rgb);
}

Sprite::~Sprite()
{

}

void Sprite::update(const uint8_t pixels[], bool flip)
{
    uint8_t pixels_rgba[this->width * this->height * 4];
    for (int y = 0; y < this->height; y++)
    {
        for (int x = 0; x < this->width; x++)
        {
            uint8_t color = pixels[x + y * this->width] * 85;
            int i = (x + y * this->width) * 4;
            if (flip) { i = (x + (this->height - 1 -y) * this->width) * 4; }
            for (int c = 0; c < 4; c++)
            {
                pixels_rgba[i + c] = color;
            }
        }
    }

    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->width, this->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_rgba);
}

const GLuint &Sprite::get_texture()
{
    return this->texture;
}

} // namespace emulator
