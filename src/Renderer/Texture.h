#pragma once

#include <string>

#include <glad/glad.h>

class Texture
{
public:
    Texture(const std::string& filepath);
    Texture(uint32_t width, uint32_t height);

    virtual ~Texture();

    void Bind(uint32_t slot = 0);

    void SetData(void* data, uint32_t size);

    uint32_t GetWidth() const { return m_width; }
    uint32_t GetHeight() const { return m_height; }

    bool operator==(const Texture& other) const { return m_rendererID == other.m_rendererID; }
private:
    uint32_t m_rendererID;

    uint32_t m_width, m_height;
    GLenum m_dataFormat, m_internalFormat;
};