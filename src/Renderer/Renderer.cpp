#include "Renderer/Renderer.h"

#include <glad/glad.h>

#include "Core/Common.h"

#include <glm/gtc/matrix_transform.hpp>

Renderer::RenderData* Renderer::s_data = nullptr;
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

    LOG_INFO("OpenGL Version {0}.{1}", major, minor);

    s_width = width;
    s_height = height;

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glDisable(GL_CULL_FACE);
    glDisable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_SCISSOR_TEST);

    glViewport(0, 0, width, height);

    s_data = new RenderData();

    for (int i = 0; i < s_data->layers.size(); i++)
    {
        s_data->layers[i] = std::make_shared<Framebuffer>(width, height);
    }

    s_data->quadRenderer = std::make_shared<QuadRendererable>();
    s_data->textRenderer = std::make_shared<TextRendererable>();

    s_data->fboShader = std::make_shared<Shader>("assets/shaders/FBO.glsl");
    s_data->fboShader->Bind();
    s_data->fboShader->SetInt("u_Texture", 0);

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

    for (int i = 0; i < s_data->layers.size(); i++)
    {
        s_data->layers[i]->Resize(width, height);
    }
}

void Renderer::BeginFrame()
{
    s_data->quadRenderer->Reset();
    s_data->textRenderer->Reset();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& layer : s_data->layers)
        layer->Clear(0, 0, 0, 0);
}

void Renderer::EndFrame()
{
    glScissor(0, 0, s_width, s_height);

    s_data->quadRenderer->Flush();
    s_data->textRenderer->Flush();

    s_data->fboShader->Bind();

    for (int i = s_data->layers.size() - 1; i >= 0; i--)
    {
        s_data->layers[i]->BindTexture();
        glBindVertexArray(s_data->fboVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }
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

float Renderer::DrawText(const glm::vec2& pos, const std::string& text, Ref<Font> font, const float scale, const glm::vec4& color)
{
    float width = 0;

    glm::vec2 currentPos = pos;
    glm::vec2 prevPos = pos;

    char* curChar = (char*)text.c_str();
    char* prevChar = nullptr;

    DrawCharacter(currentPos, font, curChar, prevChar, scale, color);
    prevChar = curChar;

    width += currentPos.x - prevPos.x;
    prevPos = currentPos;

    for (int i = 1; i < text.length(); i++)
    {
        curChar = (char*)text.c_str() + i;
        DrawCharacter(currentPos, font, curChar, prevChar, scale, color);
        prevChar = curChar;

        width += currentPos.x - prevPos.x;
        prevPos = currentPos;
    }

    return width;
}

void Renderer::DrawCharacter(glm::vec2& pos, Ref<Font> font, const char* curr, const char* prev, const float scale, const glm::vec4& color)
{
    if (s_data->textRenderer->GetIndexCount() >= TextRendererable::MaxTextIndices)
        s_data->textRenderer->FlushAndReset();

    auto glyph = font->GetCharacter(curr);
    if (prev)
    {
        pos.x += ftgl::texture_glyph_get_kerning(glyph, prev);
    }

    float textureIndex = s_data->textRenderer->GetTextureIndex(font);

    float x0 = pos.x + (glyph->offset_x * scale);
    float y0 = pos.y + (glyph->offset_y * scale);
    float x1 = x0 + (glyph->width * scale);
    float y1 = y0 - (glyph->height * scale);

    float scaledHeight = glyph->offset_y * scale;

    glm::vec2 quadPositions[4];
    quadPositions[0] = { x0, y0 };
    quadPositions[1] = { x0, y1 };
    quadPositions[2] = { x1, y1 };
    quadPositions[3] = { x1, y0 };

    glm::vec2 quadUVs[4];
    quadUVs[0] = { glyph->s0, glyph->t0 };
    quadUVs[1] = { glyph->s0, glyph->t1 };
    quadUVs[2] = { glyph->s1, glyph->t1 };
    quadUVs[3] = { glyph->s1, glyph->t0 };

    for (uint32_t i = 0; i < 4; i++)
    {
        TextVertex vertex;

        vertex.position = quadPositions[i];
        vertex.color = color;
        vertex.uvCoord = quadUVs[i];
        vertex.texIndex = textureIndex;
        vertex.renderMode = font->GetFlags() & FONT_NORMAL ? 0.0f : 1.0f;

        s_data->textRenderer->AddData(vertex);
    }

    pos.x += glyph->advance_x * scale;
    // pos.y += glyph->advance_y;
}