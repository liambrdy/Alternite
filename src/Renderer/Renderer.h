#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include "Renderer/Shader.h"

class Renderer
{
public:
    static void Init(uint32_t width, uint32_t height);
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginFrame();
    static void EndFrame();

    static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
private:
    static void FlushAndReset();

    static void SetBufferAttributeLayout(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset);
private:
    struct QuadVertex
    {
        glm::vec2 position;
        glm::vec4 color;
    };

    struct RenderData
    {
        static const int MaxQuads = 20000;
        static const int MaxVertices = MaxQuads * 4;
        static const int MaxIndices = MaxQuads * 6;

        uint32_t quadVAO, quadVBO, quadEBO;
        Shader* shader;

        Renderer::QuadVertex* quadVertexBufferPtr = nullptr;
        Renderer::QuadVertex* quadVertexBufferBase = nullptr;

        uint32_t quadIndexCount = 0;
    };

    static RenderData s_data;
    static uint32_t s_width, s_height;
};