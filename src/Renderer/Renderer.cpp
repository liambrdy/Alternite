#include "Renderer/Renderer.h"

#include <glad/glad.h>

#include "Core/Common.h"

#include <glm/gtc/matrix_transform.hpp>

Renderer::RenderData* Renderer::s_data = nullptr;

uint32_t Renderer::s_width;
uint32_t Renderer::s_height;
Ref<Camera> Renderer::s_camera;

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

    s_data->quadRenderer = std::make_shared<QuadRendererable>();
    s_data->textRenderer = std::make_shared<TextRendererable>();

    s_camera = std::make_shared<Camera>(0, width, 0, height);
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

    s_camera->SetProjection(0, width, 0, height);
}

void Renderer::BeginFrame()
{
    s_data->s_requests.clear();

    s_data->quadRenderer->Reset();
    s_data->textRenderer->Reset();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

struct
{
    bool operator() (const RenderRequest& req1, const RenderRequest& req2)
    {
        int type1 = static_cast<int>(req1.type);
        int type2 = static_cast<int>(req2.type);

        return type1 > type2;
    }
} comparator;

void Renderer::EndFrame()
{
    glScissor(0, 0, s_width, s_height);

    std::sort(s_data->s_requests.begin(), s_data->s_requests.end(), comparator);

    for (int i = 0; i < s_data->s_requests.size(); i++)
    {
        RenderRequest& request = s_data->s_requests[i];

        switch (request.type)
        {
            case RENDER_REQUEST_quad:
            {
                if (s_data->quadRenderer->GetIndexCount() >= QuadRendererable::MaxQuadIndices)
                    s_data->quadRenderer->FlushAndReset();

                float texIndex = 0.0f;
                float tiling = request.quad.tiling;

                if (request.quad.texture)
                {
                    texIndex = s_data->quadRenderer->GetTextureIndex(request.quad.texture);
                }

                glm::vec2 uvs[4] = {
                    { request.quad.texCoords[0] },
                    { request.quad.texCoords[1].x,  request.quad.texCoords[0].y },
                    { request.quad.texCoords[1].x,  request.quad.texCoords[1].y },
                    { request.quad.texCoords[0].x,  request.quad.texCoords[1].y },
                };

                for (uint32_t j = 0; j < 4; j++)
                {
                    QuadVertex vertex;
                    vertex.position = request.quad.positions[j];
                    vertex.color = request.quad.color;
                    vertex.texCoord = uvs[j];
                    vertex.texIndex = texIndex;
                    vertex.tilingFactor = tiling;

                    s_data->quadRenderer->AddData(vertex);
                }
            } break;

            case RENDER_REQUEST_text:
            {
                if (s_data->textRenderer->GetIndexCount() >= TextRendererable::MaxTextIndices)
                    s_data->textRenderer->FlushAndReset();

                float texIndex = s_data->textRenderer->GetTextureIndex(request.text.font);

                for (uint32_t j = 0; j < 4; j++)
                {
                    TextVertex vertex;

                    vertex.position = request.text.positions[j];
                    vertex.color = request.text.color;
                    vertex.uvCoord = request.text.uvs[j];
                    vertex.texIndex = texIndex;
                    vertex.renderMode = request.text.font->GetFlags() & FONT_NORMAL ? 0.0f : 1.0f;

                    s_data->textRenderer->AddData(vertex);
                }
            } break;
        }
    }

    s_data->quadRenderer->Flush();
    s_data->textRenderer->Flush();
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color)
{
    if (IsNotInScene(pos, size))
        return;

    glm::vec2 p1 = { pos.x, pos.y };
    glm::vec2 p2 = { pos.x + size.x, pos.y };
    glm::vec2 p3 = { pos.x + size.x, pos.y + size.y };
    glm::vec2 p4 = { pos.x, pos.y + size.y };

    DrawQuad(p1, p2, p3, p4, color);
}

void Renderer::DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, const glm::vec4 color)
{
    RenderRequest request = {};

    request.type = RENDER_REQUEST_quad;
    request.quad.positions[0] = p1;
    request.quad.positions[1] = p2;
    request.quad.positions[2] = p3;
    request.quad.positions[3] = p4;
    request.quad.color = color;
    request.quad.texture = nullptr;
    request.quad.tiling = 1;

    s_data->s_requests.push_back(request);
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, Ref<Texture> texture, float tiling, glm::vec4 tint)
{
    if (IsNotInScene(pos, size))
        return;

    glm::vec2 p1 = { pos.x, pos.y };
    glm::vec2 p2 = { pos.x + size.x, pos.y };
    glm::vec2 p3 = { pos.x + size.x, pos.y + size.y };
    glm::vec2 p4 = { pos.x, pos.y + size.y };

    glm::vec2 uv1 = { 0, 0 };
    glm::vec2 uv2 = { 1, 1 };

    DrawQuad(p1, p2, p3, p4, uv1, uv2, texture, tiling, tint);
}

void Renderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& uv1, const glm::vec2& uv2, Ref<Texture> texture, float tiling, glm::vec4 tint)
{
    if (IsNotInScene(pos, size))
        return;

    glm::vec2 p1 = { pos.x, pos.y };
    glm::vec2 p2 = { pos.x + size.x, pos.y };
    glm::vec2 p3 = { pos.x + size.x, pos.y + size.y };
    glm::vec2 p4 = { pos.x, pos.y + size.y };

    DrawQuad(p1, p2, p3, p4, uv1, uv2, texture, tiling, tint);
}

void Renderer::DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, const glm::vec2& uv1, const glm::vec2& uv2, Ref<Texture> texture, float tiling, glm::vec4 tint)
{
    RenderRequest request = {};
    
    request.type = RENDER_REQUEST_quad;
    request.quad.positions[0] = p1;
    request.quad.positions[1] = p2;
    request.quad.positions[2] = p3;
    request.quad.positions[3] = p4;
    request.quad.texCoords[0] = uv1;
    request.quad.texCoords[1] = uv2;
    request.quad.color = tint;
    request.quad.texture = texture;
    request.quad.tiling = tiling;

    s_data->s_requests.push_back(request);
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
    auto glyph = font->GetCharacter(curr);
    if (prev)
    {
        pos.x += ftgl::texture_glyph_get_kerning(glyph, prev);
    }

    float x0 = pos.x + (glyph->offset_x * scale);
    float y0 = pos.y + (glyph->offset_y * scale);
    float x1 = x0 + (glyph->width * scale);
    float y1 = y0 - (glyph->height * scale);

    RenderRequest request = {};

    request.type = RENDER_REQUEST_text;
    request.text.positions[0] = { x0, y0 };
    request.text.positions[1] = { x0, y1 };
    request.text.positions[2] = { x1, y1 };
    request.text.positions[3] = { x1, y0 };
    request.text.uvs[0] = { glyph->s0, glyph->t0 };
    request.text.uvs[1] = { glyph->s0, glyph->t1 };
    request.text.uvs[2] = { glyph->s1, glyph->t1 };
    request.text.uvs[3] = { glyph->s1, glyph->t0 };
    request.text.color = color;
    request.text.font = font;

    s_data->s_requests.push_back(request);

    pos.x += glyph->advance_x * scale;
    // pos.y += glyph->advance_y;
}

bool Renderer::IsNotInScene(const glm::vec2& pos, const glm::vec2& size)
{
    glm::vec2 camPos = s_camera->GetPosition();

    bool vertBounds = pos.x + size.x < camPos.x || pos.x > camPos.x + s_width;
    bool horizBounds = pos.y + size.y < camPos.y || pos.y > camPos.y + s_height;

    return vertBounds || horizBounds;
}