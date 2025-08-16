#include "gameboy-emulator/graphics/screen.hpp"

#include <GL/glew.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>

namespace emulator {

const char *Screen::vertex_s = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 TexCoord;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";

const char *Screen::fragment_s = R"(
    #version 330 core
    in vec2 TexCoord;

    out vec4 FragColor;

    uniform sampler2D s_texture;

    void main()
    {
        FragColor = texture(s_texture, TexCoord);
        // FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)";

const float Screen::vertices[5 * 4] = {
    // positions         // texture coords
    0.f, 0.f, 0.f,       0.f, 0.f,      // bottom left
    160.f, 0.f, 0.f,     160.f, 0.f,    // bottom right
    160.f, 144.f, 0.f,   160.f, 144.f,  // top right
    0.f, 144.f, 0.f,     0.f, 144.f     // top left
};

const unsigned int Screen::indices[3 * 2] = {
    0, 1, 2,   // first triangle
    0, 2, 3    // second triangle
};

uint8_t Screen::pixels[160 * 144] = {};

unsigned int Screen::texture;
unsigned int Screen::VAO;
unsigned int Screen::program;

void Screen::update_texture()
{
    uint8_t pixels_rgb[160 * 144 * 3];
    for (int y = 0; y < 144; y++) // y position
    {
        for (int x = 0; x < 160; x++) // x position
        {
            uint8_t color = pixels[x + y * 160] * 85;
            for (int c = 0; c < 3; c++) // color channel
            {
                pixels_rgb[(x + (143 - y) * 160) * 3 + c] = color;
            }
        }
    }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 160, 144, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels_rgb);
}

void Screen::init()
{
    //////// create shaders
    // vertex shader
    unsigned int vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_s, NULL);
    glCompileShader(vertex);
    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_s, NULL);
    glCompileShader(fragment);
    // program
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    //////// create objects
    unsigned int VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //////// link attributes
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //////// create texture
    uint8_t pixels_rgb[160 * 144 * 3] = { 128 };
    glGenTextures(1, &texture);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 160, 144, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels_rgb);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Screen::render()
{
    glUseProgram(program);
    // texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    update_texture();
    // vertices
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

} // namespace emulator
