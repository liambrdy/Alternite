#pragma once

#include <cstdint>
#include <array>

#include <glm/glm.hpp>

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Text/Font.h"
#include "Renderer/Framebuffer.h"
#include "Renderer/Camera.h"

#include "Renderer/Rendererable.h"

#include "Core/Common.h"

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
    static void DrawQuad(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const glm::vec2& p4, Ref<Texture> texture, float tiling = 1.0f, glm::vec4 tint = glm::vec4(1.0f));

    static float DrawText(const glm::vec2& pos, const std::string& text, Ref<Font> font, const float scale, const glm::vec4& color = glm::vec4(1.0f));
    static void DrawCharacter(glm::vec2& pos, Ref<Font> font, const char* curr, const char* prev, const float scale, const glm::vec4& color);

    enum Layer
    {
        LAYER_game = 0,
        LAYER_gui,
        LAYER_MAX
    };

    static std::array<Ref<Framebuffer>, LAYER_MAX - 1> GetLayers() { return s_data->layers; }

    static Ref<Camera> GetCamera() { return s_camera; }
private:
    struct RenderData
    {
        std::array<Ref<Framebuffer>, LAYER_MAX - 1> layers;

        Ref<QuadRendererable> quadRenderer;
        Ref<TextRendererable> textRenderer;
        
        uint32_t fboVAO;
        Ref<Shader> fboShader;
    };

    static RenderData* s_data;
    static uint32_t s_width, s_height;
    static Ref<Camera> s_camera;
};