#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include <glm/glm.hpp>

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Text/Font.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Camera.h"

#include "Renderer/Rendererable.h"

#include "Core/Common.h"

enum RenderRequestType
{
    RENDER_REQUEST_quad,
    RENDER_REQUEST_text
};

struct RenderRequest
{
    RenderRequestType type;

    struct
    {
        glm::vec2 positions[4];
        glm::vec4 color;
        glm::vec2 texCoords[2];
        float tiling;
        Ref<Texture> texture;
    } quad;

    struct
    {
        glm::vec2 positions[4];
        glm::vec2 uvs[4];
        glm::vec4 color;
        float mode;
        Ref<Font> font;
    } text;
};

class Renderer
{
public:
    static void Init(uint32_t width, uint32_t height);
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void SetRenderOrigin(const glm::vec2& pos) { glm::vec2 newPos = { pos.x - s_width / 2, pos.y - s_height / 2 } ; s_camera->SetPosition(newPos); }

    static void BeginFrame();
    static void EndFrame();

    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, const glm::vec4 color);

    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, Ref<Texture> texture, float tiling = 1.0f, glm::vec4 tint = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec2& uv1, const glm::vec2& uv2, Ref<Texture> texture, float tiling = 1.0f, glm::vec4 tint = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, const glm::vec2& uv1, const glm::vec2& uv2, Ref<Texture> texture, float tiling = 1.0f, glm::vec4 tint = glm::vec4(1.0f));

    static float DrawText(const glm::vec2& pos, const std::string& text, Ref<Font> font, const float scale, const glm::vec4& color = glm::vec4(1.0f));
    static void DrawCharacter(glm::vec2& pos, Ref<Font> font, const char* curr, const char* prev, const float scale, const glm::vec4& color);

    static Ref<Camera> GetCamera() { return s_camera; }
private:
    static bool IsNotInScene(const glm::vec2& pos, const glm::vec2& size);
    static glm::vec2 ConvertToPixels(const glm::vec2& vec);
private:
    static constexpr uint32_t PPM = 32;

    struct RenderData
    {
        Ref<QuadRendererable> quadRenderer;
        Ref<TextRendererable> textRenderer;

        std::vector<RenderRequest> s_requests;
    };

    static RenderData* s_data;
    static uint32_t s_width, s_height;
    static Ref<Camera> s_camera;
};
