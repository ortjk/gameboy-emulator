#include "gameboy-emulator/graphics/screen.hpp"

#include <GL/glew.h>
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
    }
)";

const float Screen::vertices[5 * 4] = {
    // positions         // texture coords
    0.f, 144.f, 0.f,     0, 144.f,       // bottom left
    160.f, 144.f, 0.f,   160.f, 144.f,   // bottom right
    160.f, 0.f, 0.f,     160.f, 0.f,     // top right
    0.f, 0.f, 0.f,       0, 0.f,         // top left
};

const unsigned int Screen::indices[3 * 2] = {
    0, 1, 2,   // first triangle
    0, 2, 3    // second triangle
};

std::unique_ptr<Sprite> Screen::screen_sprite;
unsigned int Screen::VAO;
unsigned int Screen::program;

uint8_t Screen::pixels[160 * 144] = {};

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

    //////// link texture
    screen_sprite = std::make_unique<Sprite>(160, 144);
}

void Screen::render()
{
    glUseProgram(program);
    screen_sprite->update(pixels, true);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

} // namespace emulator
