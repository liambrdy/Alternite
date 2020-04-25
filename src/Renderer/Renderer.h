#pragma once

#include <cstdint>
#include <array>

#include <glm/glm.hpp>

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"

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
    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, Ref<Texture> texture, float tiling = 1.0f, glm::vec4 tint = glm::vec4(1.0f));
private:
    static void FlushAndReset();

    static void SetBufferAttributeLayout(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset);
private:
    struct QuadVertex
    {
        glm::vec2 position;
        glm::vec4 color;
        glm::vec2 uvCoord;
        float texIndex;
        float tilingFactor;
    };

    struct RenderData
    {
        static const uint32_t MaxQuads = 20000;
        static const uint32_t MaxVertices = MaxQuads * 4;
        static const uint32_t MaxIndices = MaxQuads * 6;
        static const uint32_t MaxTextureSlots = 32;

        uint32_t quadVAO, quadVBO, quadEBO;
        Ref<Shader> shader;
        Ref<Texture> whiteTexture;
        
        std::array<Ref<Texture>, MaxTextureSlots> textureSlots;
        uint32_t textureSlotIndex = 1;

        Renderer::QuadVertex* quadVertexBufferPtr = nullptr;
        Renderer::QuadVertex* quadVertexBufferBase = nullptr;

        uint32_t quadIndexCount = 0;
    };

    static RenderData* s_data;
    static uint32_t s_width, s_height;
};