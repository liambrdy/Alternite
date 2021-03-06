#pragma once

#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Camera.h"
#include "Renderer/Text/Font.h"

#include "Core/Common.h"

#include <array>

class Rendererable
{
public:
    virtual ~Rendererable() = default;

    virtual void Reset() = 0;
    virtual void Flush() = 0;
protected:
    uint32_t m_VAO, m_VBO, m_EBO;

    uint32_t m_indexCount;

    Ref<Shader> m_shader;

    static const uint32_t MaxTextureSlots = 32;
};

struct QuadVertex
{
    glm::vec2 position;
    glm::vec4 color;
    glm::vec2 texCoord;
    float texIndex;
    float tilingFactor;
};

class QuadRendererable : public Rendererable
{
public:
    QuadRendererable();
    virtual ~QuadRendererable();

    void AddData(QuadVertex data);

    float GetTextureIndex(Ref<Texture> texture);

    virtual void Reset() override;
    virtual void Flush() override;

    void FlushAndReset() { Flush(); Reset(); }

    uint32_t GetIndexCount() const { return m_indexCount; }
public:
    static const uint32_t MaxQuads = 2000;
    static const uint32_t MaxQuadVertices = MaxQuads * 4;
    static const uint32_t MaxQuadIndices = MaxQuads * 6;
private:
    QuadVertex* m_vertexBase = nullptr;
    QuadVertex* m_vertexPtr = nullptr;

    uint32_t m_vertexCount = 0;

    std::array<Ref<Texture>, 32> m_textureSlots;
    Ref<Texture> m_whiteTexture;

    uint32_t m_textureSlotIndex = 1;
};

struct TextVertex
{
    glm::vec2 position;
    glm::vec2 uvCoord;
    glm::vec4 color;
    float texIndex;
    float renderMode;
};

class TextRendererable : public Rendererable
{
public:
    TextRendererable();
    virtual ~TextRendererable();

    void AddData(TextVertex data);

    virtual void Reset() override;
    virtual void Flush() override;

    float GetTextureIndex(Ref<Font> font);

    void FlushAndReset() { Flush(); Reset(); }

    uint32_t GetIndexCount() const { return m_indexCount; }
public:
    static const uint32_t MaxTexts = 20000;
    static const uint32_t MaxTextVertices = MaxTexts * 4;
    static const uint32_t MaxTextIndices = MaxTexts * 6;
private:
    TextVertex* m_vertexBase = nullptr;
    TextVertex* m_vertexPtr = nullptr;

    uint32_t m_fontIndex;
    std::array<Ref<Font>, 32> m_fonts;

    uint32_t m_vertexCount = 0;
};