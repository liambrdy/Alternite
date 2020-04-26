#pragma once

#include <cstdint>
#include <array>

#include <glm/glm.hpp>

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Font.h"
#include "Renderer/Framebuffer.h"

#include "Renderer/Rendererable.h"

#include "Core/Common.h"

class Renderer
{
public:
    static void Init(uint32_t width, uint32_t height);
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginFrame();
    static void EndFrame();

    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
    static void DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, const glm::vec4 color);

    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, Ref<Texture> texture, float tiling = 1.0f, glm::vec4 tint = glm::vec4(1.0f));
    static void DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, Ref<Texture> texture, float tiling = 1.0f, glm::vec4 tint = glm::vec4(1.0f));

    static void DrawText(const glm::vec2& pos, const std::string& text, Ref<Font> font, const glm::vec4& color = glm::vec4(1.0f));
    static void DrawCharacter(const glm::vec2& pos, Character character, const glm::vec4& color);

    static std::array<Ref<Framebuffer>, 1> GetLayers() { return s_data->layers; }
private:
    struct RenderData
    {
        Ref<Framebuffer> guiFramebuffer;
        std::array<Ref<Framebuffer>, 1> layers;

        Ref<QuadRendererable> quadRenderer;
        Ref<TextRendererable> textRenderer;
        
        uint32_t fboVAO;
        Ref<Shader> fboShader;
    };

    static RenderData* s_data;
    static uint32_t s_width, s_height;
};