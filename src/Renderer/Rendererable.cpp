#include "Renderer/Rendererable.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer/Renderer.h"

static void SetBufferAttributeLayout(uint32_t index, uint32_t size, uint32_t stride, uint32_t offset)
{
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (const void*)(offset * sizeof(float)));
}

void Rendererable::BindFramebuffer()
{
    if (m_layer == 0)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    else
    {
        auto layers = Renderer::GetLayers();
        layers[m_layer - 1]->Bind();
    }
}

QuadRendererable::QuadRendererable()
{
    m_layer = static_cast<uint32_t>(Renderer::LAYER_game);

    m_vertexBase = new QuadVertex[MaxQuadVertices];

    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glCreateBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, MaxQuadVertices * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);

    SetBufferAttributeLayout(0, 2, 10, 0);
    SetBufferAttributeLayout(1, 4, 10, 2);
    SetBufferAttributeLayout(2, 2, 10, 6);
    SetBufferAttributeLayout(3, 1, 10, 8);
    SetBufferAttributeLayout(4, 1, 10, 9);

    uint32_t* quadIndices = new uint32_t[MaxQuadIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < MaxQuadIndices; i += 6)
    {
        quadIndices[i + 0] = offset + 0;
        quadIndices[i + 1] = offset + 1;
        quadIndices[i + 2] = offset + 2;

        quadIndices[i + 3] = offset + 2;
        quadIndices[i + 4] = offset + 3;
        quadIndices[i + 5] = offset + 0;

        offset += 4;
    }

    glCreateBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxQuadIndices * sizeof(uint32_t), quadIndices, GL_STATIC_DRAW);

    delete[] quadIndices;
    
    int32_t samplers[MaxTextureSlots];
    for (int32_t i = 0; i < MaxTextureSlots; i++)
        samplers[i] = i;

    m_shader = std::make_shared<Shader>("assets/shaders/Quad.glsl");

    m_shader->Bind();
    m_shader->SetIntArray("u_Textures", samplers, MaxTextureSlots);

    m_whiteTexture = std::make_shared<Texture>(1, 1);
    uint32_t whiteTextureData = 0xffffffff;
    m_whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

    m_textureSlots[0] = m_whiteTexture;

    glBindVertexArray(0);
}

QuadRendererable::~QuadRendererable()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);

    delete[] m_vertexBase;
}

void QuadRendererable::AddData(QuadVertex data)
{
    *m_vertexPtr = data;
    m_vertexPtr++;

    if (m_vertexCount == 3)
    {
        m_indexCount += 6;
        m_vertexCount = 0;
    }
    else 
        m_vertexCount++;
}

float QuadRendererable::GetTextureIndex(Ref<Texture> texture)
{
    float textureIndex = 0.0f;
    for (uint32_t i = 1; i < m_textureSlotIndex; i++)
    {
        if (*m_textureSlots[i].get() == *texture.get())
        {
            textureIndex = (float)i;
            break;
        }
    }

    if (textureIndex == 0.0f)
    {
        if (m_textureSlotIndex >= MaxTextureSlots)
            FlushAndReset();

        textureIndex = (float)m_textureSlotIndex;
        m_textureSlots[m_textureSlotIndex] = texture;
        m_textureSlotIndex++;
    }
    
    return textureIndex;
}

void QuadRendererable::Reset()
{
    m_indexCount = 0;
    m_textureSlotIndex = 1;

    m_vertexPtr = m_vertexBase;
}

void QuadRendererable::Flush()
{
    BindFramebuffer();

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    ptrdiff_t size = (uint8_t*)m_vertexPtr - (uint8_t*)m_vertexBase;
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_vertexBase);

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

    for (uint32_t i = 0; i < m_textureSlotIndex; i++)
    {
        m_textureSlots[i]->Bind(i);
    }

    m_shader->Bind();
    m_shader->SetMat4("u_Projection", projection);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

TextRendererable::TextRendererable()
{
    m_layer = static_cast<uint32_t>(Renderer::LAYER_gui);

    m_vertexBase = new TextVertex[MaxTextVertices];

    glCreateVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glCreateBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, MaxTextVertices * sizeof(TextVertex), nullptr, GL_DYNAMIC_DRAW);

    SetBufferAttributeLayout(0, 2, 8, 0);
    SetBufferAttributeLayout(1, 2, 8, 2);
    SetBufferAttributeLayout(2, 4, 8, 4);

    uint32_t* textIndices = new uint32_t[MaxTextIndices];

    uint32_t offset = 0;
    for (uint32_t i = 0; i < MaxTextIndices; i += 6)
    {
        textIndices[i + 0] = offset + 0;
        textIndices[i + 1] = offset + 1;
        textIndices[i + 2] = offset + 2;

        textIndices[i + 3] = offset + 2;
        textIndices[i + 4] = offset + 3;
        textIndices[i + 5] = offset + 0;

        offset += 4;
    }

    glCreateBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, MaxTextIndices * sizeof(uint32_t), textIndices, GL_STATIC_DRAW);

    delete[] textIndices;

    m_shader = std::make_shared<Shader>("assets/shaders/Text.glsl");

    m_shader->Bind();
    m_shader->SetInt("u_Texture", 0);

    glBindVertexArray(0);
}

TextRendererable::~TextRendererable()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    glDeleteVertexArrays(1, &m_VAO);

    delete[] m_vertexBase;
}

void TextRendererable::AddData(TextVertex data)
{
    *m_vertexPtr = data;
    m_vertexPtr++;

    if (m_vertexCount == 3)
    {
        m_indexCount += 6;
        m_vertexCount = 0;
    }
    else 
        m_vertexCount++;
}

void TextRendererable::Reset()
{
    m_indexCount = 0;

    m_vertexPtr = m_vertexBase;
}

void TextRendererable::Flush()
{
    BindFramebuffer();

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    ptrdiff_t size = (uint8_t*)m_vertexPtr - (uint8_t*)m_vertexBase;
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, m_vertexBase);

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);

    m_font->BindTexture();

    m_shader->Bind();
    m_shader->SetMat4("u_Projection", projection);

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}