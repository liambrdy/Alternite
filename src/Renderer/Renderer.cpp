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
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
    glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
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

    SetBufferAttributeLayout(0, 2, 10, 0);
    SetBufferAttributeLayout(1, 4, 10, 2);
    SetBufferAttributeLayout(2, 2, 10, 6);
    SetBufferAttributeLayout(3, 1, 10, 8);
    SetBufferAttributeLayout(4, 1, 10, 9);

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

    s_data.whiteTexture = new Texture(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    s_data.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    int32_t samplers[s_data.MaxTextureSlots];
    for (int32_t i = 0; i < s_data.MaxTextureSlots; i++)
        samplers[i] = i;

    s_data.shader = new Shader("assets/shaders/Quad.glsl");
    s_data.shader->Bind();
    s_data.shader->SetIntArray("u_Textures", samplers, s_data.MaxTextureSlots);

    s_data.textureSlots[0] = s_data.whiteTexture;

    // s_data.quadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
    // s_data.quadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
    // s_data.quadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
    // s_data.quadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
}

void Renderer::Shutdown()
{
    delete s_data.shader;
    delete s_data.whiteTexture;

    glDeleteBuffers(1, &s_data.quadEBO);
    glDeleteBuffers(1, &s_data.quadVBO);
    glDeleteVertexArrays(1, &s_data.quadVAO);

    delete[] s_data.quadVertexBufferBase;
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

    s_data.textureSlotIndex = 1;

    glDisable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndFrame()
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, s_width, s_height);

    glBindBuffer(GL_ARRAY_BUFFER, s_data.quadVBO);
    ptrdiff_t size = (uint8_t*)s_data.quadVertexBufferPtr - (uint8_t*)s_data.quadVertexBufferBase;
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_data.quadVertexBufferBase);

    glm::mat4 projectionMat = glm::ortho(0.0f, (float)s_width, 0.0f, (float)s_height, -1.f, 1.0f);

    for (uint32_t i = 0; i < s_data.textureSlotIndex; i++)
    {
        s_data.textureSlots[i]->Bind(i);
    }

    s_data.shader->Bind();
    s_data.shader->SetMat4("u_Projection", projectionMat);

    glBindVertexArray(s_data.quadVAO);
    glDrawElements(GL_TRIANGLES, s_data.quadIndexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
    if (s_data.quadIndexCount >= s_data.MaxIndices)
        FlushAndReset();

    glm::vec2 quadPositions[4];
    quadPositions[0] = { pos.x, pos.y };
    quadPositions[1] = { pos.x + size.x, pos.y };
    quadPositions[2] = { pos.x + size.x, pos.y + size.y };
    quadPositions[3] = { pos.x, pos.y + size.y };

    glm::vec2 quadUVs[4];
    quadUVs[0] = { 0.0f, 0.0f };
    quadUVs[1] = { 1.0f, 0.0f };
    quadUVs[2] = { 1.0f, 1.0f };
    quadUVs[3] = { 0.0f, 1.0f };

    for (uint32_t i = 0; i < 4; i++)
    {
        s_data.quadVertexBufferPtr->position = quadPositions[i];
        s_data.quadVertexBufferPtr->color = color;
        s_data.quadVertexBufferPtr->uvCoord = quadUVs[i];
        s_data.quadVertexBufferPtr->texIndex = 0;
        s_data.quadVertexBufferPtr->tilingFactor = 1.0f;

        s_data.quadVertexBufferPtr++;
    }

    s_data.quadIndexCount += 6;
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, Texture* texture, float tiling, glm::vec4 tint)
{
    if (s_data.quadIndexCount >= s_data.MaxIndices)
        FlushAndReset();

    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < s_data.textureSlotIndex; i++)
    {
        if (*s_data.textureSlots[i] == *texture)
        {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        textureIndex = (float)s_data.textureSlotIndex;
        s_data.textureSlots[s_data.textureSlotIndex] = texture;
        s_data.textureSlotIndex++;
    }

    glm::vec2 quadPositions[4];
    quadPositions[0] = { pos.x, pos.y };
    quadPositions[1] = { pos.x + size.x, pos.y };
    quadPositions[2] = { pos.x + size.x, pos.y + size.y };
    quadPositions[3] = { pos.x, pos.y + size.y };

    glm::vec2 quadUVs[4];
    quadUVs[0] = { 0.0f, 0.0f };
    quadUVs[1] = { 1.0f, 0.0f };
    quadUVs[2] = { 1.0f, 1.0f };
    quadUVs[3] = { 0.0f, 1.0f };

    for (uint32_t i = 0; i < 4; i++)
    {
        s_data.quadVertexBufferPtr->position = quadPositions[i];
        s_data.quadVertexBufferPtr->color = tint;
        s_data.quadVertexBufferPtr->uvCoord = quadUVs[i];
        s_data.quadVertexBufferPtr->texIndex = textureIndex;
        s_data.quadVertexBufferPtr->tilingFactor = tiling;

        s_data.quadVertexBufferPtr++;
    }

    s_data.quadIndexCount += 6;
}

void Renderer::FlushAndReset()
{
    EndFrame();

    s_data.quadVertexBufferPtr = s_data.quadVertexBufferBase;
    s_data.quadIndexCount = 0;
    
    s_data.textureSlotIndex = 1;
}

void Renderer::SetBufferAttributeLayout(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (const void*)(offset * sizeof(float)));
}