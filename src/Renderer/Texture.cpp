#include "Renderer/Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Core/Common.h"

Texture::Texture(const std::string& filepath)
{
    int width, height, channels;
    stbi_uc* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
    ASSERT(pixels, "Failed to load texture: {0}", filepath);

    m_width = width;
    m_height = height;

    if (channels == STBI_rgb)
    {
        m_dataFormat = GL_RGB;
        m_internalFormat = GL_RGB8;
    }
    else if (channels == STBI_rgb_alpha)
    {
        m_dataFormat = GL_RGBA;
        m_internalFormat = GL_RGBA8;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
    glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, pixels);

    stbi_image_free(pixels);
}

Texture::Texture(uint32_t width, uint32_t height)
    : m_width(width), m_height(height)
{
    m_internalFormat = GL_RGBA8;
    m_dataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererID);
    glTextureStorage2D(m_rendererID, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_rendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_rendererID);
}

void Texture::Bind(uint32_t slot)
{
    glBindTextureUnit(slot, m_rendererID);
}

void Texture::SetData(void* data, uint32_t size)
{
    uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
    ASSERT(size == m_width * m_height * bpp, "Data must fill entire texture!");
    
    glTextureSubImage2D(m_rendererID, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}
