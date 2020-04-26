#pragma once

#include <cstdint>

#include "Core/Common.h"

class Framebuffer
{
public:
    Framebuffer(uint32_t width, uint32_t height);
    virtual ~Framebuffer();

    void Resize(uint32_t width, uint32_t height);

    void Bind() const;
    void Unbind() const;

    void BindTexture(uint32_t slot = 0) const;

    uint32_t GetWidth() const { return m_width; }
    uint32_t GetHeight() const { return m_height; }
private:
    uint32_t m_rendererID;
    uint32_t m_colorAttachment;

    uint32_t m_width, m_height;
};