#include "Renderer/Renderer.h"

#include <glad/glad.h>

#include "Core/Common.h"

#include <glm/gtc/matrix_transform.hpp>

Renderer::RenderData Renderer::s_data;
uint32_t Renderer::s_width;
uint32_t Renderer::s_height;

const char* vertSrc = "#version 460 core\n"
"layout (location = 0) in vec2 a_Position;\n"
"layout (location = 1) in vec4 a_Color;\n"
"out vec4 v_Color;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);\n"
"   v_Color = a_Color;\n"
"}\n";

const char* fragSrc = "#version 460 core\n"
"layout (location = 0) out vec4 color;\n"
"in vec4 v_Color;\n"
"void main()\n"
"{\n"
"    color = v_Color;\n"
"}\n";

#ifndef NDEBUG
static APIENTRY void DebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:            LOG_CRITICAL(message); return;
        case GL_DEBUG_SEVERITY_MEDIUM:          LOG_ERROR(message); return;
        case GL_DEBUG_SEVERITY_LOW:             LOG_INFO(message); return;
        case GL_DEBUG_SEVERITY_NOTIFICATION:    LOG_TRACE(message); return;
    }

    ASSERT(false, "Unknown severity level!");
}
#endif

void Renderer::Init(uint32_t width, uint32_t height)
{
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(DebugCallback, nullptr);
    // glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

    s_width = width;
    s_height = height;

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glDisable(GL_CULL_FACE);
    glDisable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, width, height);

    s_data.quadVertexBufferBase = new QuadVertex[s_data.MaxVertices];

    glCreateVertexArrays(1, &s_data.quadVAO);
    glBindVertexArray(s_data.quadVAO);

    glCreateBuffers(1, &s_data.quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_data.quadVBO);
    glBufferData(GL_ARRAY_BUFFER, s_data.MaxVertices * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

    SetBufferAttributeLayout(0, 2, 6, 0);
    SetBufferAttributeLayout(1, 4, 6, 2);

    uint32_t* quadIndices = new uint32_t[s_data.MaxIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < s_data.MaxIndices; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    glCreateBuffers(1, &s_data.quadEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_data.quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, s_data.MaxIndices * sizeof(uint32_t), quadIndices, GL_STATIC_DRAW);

    delete[] quadIndices;

    glBindVertexArray(0);

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertSrc, nullptr);
    glCompileShader(vert);

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSrc, nullptr);
    glCompileShader(frag);

    s_data.shader = glCreateProgram();
    glAttachShader(s_data.shader, vert);
    glAttachShader(s_data.shader, frag);
    glLinkProgram(s_data.shader);
    glDetachShader(s_data.shader, vert);
    glDetachShader(s_data.shader, frag);
    glDeleteShader(vert);
    glDeleteShader(frag);

    s_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    s_data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    s_data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
    s_data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height)
{
    s_width = width;
    s_height = height;
    glViewport(0, 0, width, height);
}

void Renderer::BeginFrame()
{
    s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
    s_data.quadIndexCount = 0;
}

void Renderer::EndFrame()
{
    glDisable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, s_width, s_height);

    glBindBuffer(GL_ARRAY_BUFFER, s_data.quadVBO);
    ptrdiff_t size = (uint8_t*)s_data.quadVertexBufferPtr - (uint8_t*)s_data.quadVertexBufferBase;
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_data.quadVertexBufferBase);

    glUseProgram(s_data.shader);

    glBindVertexArray(s_data.quadVAO);
    glDrawElements(GL_TRIANGLES, s_data.quadIndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
    if (s_data.quadIndexCount >= s_data.MaxIndices)
        FlushAndReset();

    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 0.0f));

    for (uint32_t i = 0; i < 4; i++)
    {
        s_data.quadVertexBufferPtr->position = transform * s_data.quadVertexPositions[i];
        s_data.quadVertexBufferPtr->color = color;

        s_data.quadVertexBufferPtr++;
    }

    s_data.quadIndexCount += 6;
}

void Renderer::FlushAndReset()
{
    EndFrame();

    s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
    s_data.quadIndexCount = 0;
}

void Renderer::SetBufferAttributeLayout(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (const void*)(offset * sizeof(float)));
}