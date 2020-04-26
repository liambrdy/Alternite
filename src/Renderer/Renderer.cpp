#include "Renderer/Renderer.h"

#include <glad/glad.h>

#include "Core/Common.h"

#include <glm/gtc/matrix_transform.hpp>

Renderer::RenderData* Renderer::s_data;
uint32_t Renderer::s_width;
uint32_t Renderer::s_height;

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

    int major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    LOG_INFO("OpenGL {0}.{1}", major, minor);

    s_width = width;
    s_height = height;

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glDisable(GL_CULL_FACE);
    glDisable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glViewport(0, 0, width, height);

    s_data = new RenderData();

    s_data->quadRenderer = std::make_shared<QuadRendererable>();
    s_data->textRenderer = std::make_shared<TextRendererable>();

    s_data->guiFramebuffer = std::make_shared<Framebuffer>(width, height);

    s_data->fboShader = std::make_shared<Shader>("assets/shaders/FBO.glsl");

    glCreateVertexArrays(1, &s_data->fboVAO);
}

void Renderer::Shutdown()
{
    delete s_data;
}

void Renderer::OnWindowResize(uint32_t width, uint32_t height)
{
    s_width = width;
    s_height = height;
    glViewport(0, 0, width, height);

    s_data->guiFramebuffer->Resize(width, height);
}

void Renderer::BeginFrame()
{
    s_data->quadRenderer->Reset();
    s_data->textRenderer->Reset();

    glDisable(GL_SCISSOR_TEST);
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndFrame()
{
    glEnable(GL_SCISSOR_TEST);
    glScissor(0, 0, s_width, s_height);

    s_data->quadRenderer->Flush();
    s_data->textRenderer->Flush();

    s_data->fboShader->Bind();
    s_data->fboShader->SetInt("u_Texture", 0);
    s_data->guiFramebuffer->BindTexture();

    glBindVertexArray(s_data->fboVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
    glm::vec2 p1 = { pos.x, pos.y };
    glm::vec2 p2 = { pos.x + size.x, pos.y };
    glm::vec2 p3 = { pos.x + size.x, pos.y + size.y };
    glm::vec2 p4 = { pos.x, pos.y + size.y };

    DrawQuad(p1, p2, p3, p4, color);
}

void Renderer::DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, const glm::vec4 color)
{
    if (s_data->quadRenderer->GetIndexCount() >= QuadRendererable::MaxQuadIndices)
        s_data->quadRenderer->FlushAndReset();

    glm::vec2 quadPositions[4];
    quadPositions[0] = p1;
    quadPositions[1] = p2;
    quadPositions[2] = p3;
    quadPositions[3] = p4;

    glm::vec2 quadUVs[4];
    quadUVs[0] = { 0.0f, 0.0f };
    quadUVs[1] = { 1.0f, 0.0f };
    quadUVs[2] = { 1.0f, 1.0f };
    quadUVs[3] = { 0.0f, 1.0f };

    for (uint32_t i = 0; i < 4; i++)
    {
        QuadVertex vertex;
        vertex.position = quadPositions[i];
        vertex.color = color;
        vertex.uvCoord = quadUVs[i];
        vertex.texIndex = 0;
        vertex.tilingFactor = 1.0f;

        s_data->quadRenderer->AddData(vertex);
    }
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, Ref<Texture> texture, float tiling, glm::vec4 tint)
{
    glm::vec2 p1 = { pos.x, pos.y };
    glm::vec2 p2 = { pos.x + size.x, pos.y };
    glm::vec2 p3 = { pos.x + size.x, pos.y + size.y };
    glm::vec2 p4 = { pos.x, pos.y + size.y };

    DrawQuad(p1, p2, p3, p4, texture, tiling, tint);
}

void Renderer::DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, Ref<Texture> texture, float tiling, glm::vec4 tint)
{
    if (s_data->quadRenderer->GetIndexCount() >= QuadRendererable::MaxQuadIndices)
        s_data->quadRenderer->FlushAndReset();

    float textureIndex = s_data->quadRenderer->GetTextureIndex(texture);

    glm::vec2 quadPositions[4];
    quadPositions[0] = p1;
    quadPositions[1] = p2;
    quadPositions[2] = p3;
    quadPositions[3] = p4;

    glm::vec2 quadUVs[4];
    quadUVs[0] = { 0.0f, 0.0f };
    quadUVs[1] = { 1.0f, 0.0f };
    quadUVs[2] = { 1.0f, 1.0f };
    quadUVs[3] = { 0.0f, 1.0f };

    for (uint32_t i = 0; i < 4; i++)
    {
        QuadVertex vertex;

        vertex.position = quadPositions[i];
        vertex.color = tint;
        vertex.uvCoord = quadUVs[i];
        vertex.texIndex = textureIndex;
        vertex.tilingFactor = tiling;

        s_data->quadRenderer->AddData(vertex);
    }
}

void Renderer::DrawText(const glm::vec2& pos, const std::string& text, Ref<Font> font, const glm::vec4& color)
{
    glm::vec2 currentPos = pos;

    for (int i = 0; i < text.length(); i++)
    {
        Character c = font->GetCharacter(text[i]);

        DrawCharacter(currentPos, c, color);

        currentPos.x += (c.advance >> 6);
    }
}

void Renderer::DrawCharacter(const glm::vec2& pos, Character c, const glm::vec4& color)
{
    if (s_data->textRenderer->GetIndexCount() >= TextRendererable::MaxTextIndices)
        s_data->textRenderer->FlushAndReset();

    float textureIndex = s_data->textRenderer->GetTextureIndex(c.texID);

    float xPos = pos.x + c.bearing.x;
    float yPos = pos.y - (c.size.y - c.bearing.y);

    glm::vec2 quadPositions[4];
    quadPositions[0] = { xPos, yPos + c.size.y };
    quadPositions[1] = { xPos + c.size.x, yPos + c.size.y };
    quadPositions[2] = { xPos + c.size.x, yPos };
    quadPositions[3] = { xPos, yPos };

    glm::vec2 quadUVs[4];
    quadUVs[0] = { 0.0f, 0.0f };
    quadUVs[1] = { 1.0f, 0.0f };
    quadUVs[2] = { 1.0f, 1.0f };
    quadUVs[3] = { 0.0f, 1.0f };    

    for (uint32_t i = 0; i < 4; i++)
    {
        TextVertex vertex;

        vertex.position = quadPositions[i];
        vertex.color = color;
        vertex.uvCoord = quadUVs[i];
        vertex.texIndex = textureIndex;

        s_data->textRenderer->AddData(vertex);
    }
}