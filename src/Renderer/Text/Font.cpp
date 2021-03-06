#include "Renderer/Text/Font.h"

#include "Core/Common.h"

#include <glad/glad.h>

Font::Font(const std::string& filepath, int flags)
    : m_flags(flags)
{
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    m_atlas = ftgl::texture_atlas_new(550, 550, 1);

    m_font = ftgl::texture_font_new_from_file(m_atlas, 72, filepath.c_str());
    if (m_flags & FONT_NORMAL)
        m_font->rendermode = ftgl::RENDER_NORMAL;
    else if (m_flags & FONT_SIGNED_DISTANCE)
        m_font->rendermode = ftgl::RENDER_SIGNED_DISTANCE_FIELD;

    for (GLubyte c = 32; c < 127; c++)
    {
        const char* codepoint = (char*)&c;
        ftgl::texture_font_load_glyph(m_font, codepoint);
    }

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_atlas->width, m_atlas->height, 0, GL_RED, GL_UNSIGNED_BYTE, m_atlas->data);
}

Font::~Font()
{
    glDeleteTextures(1, &m_texture);

    ftgl::texture_font_delete(m_font);
    ftgl::texture_atlas_delete(m_atlas);
}

ftgl::texture_glyph_t* Font::GetCharacter(const char* codepoint)
{
    return ftgl::texture_font_find_glyph(m_font, codepoint);
}

void Font::BindTexture(int slot) const
{
    glBindTextureUnit(slot, m_texture);
}

void Font::RecreateTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_atlas->width, m_atlas->height, GL_RED, GL_UNSIGNED_BYTE, m_atlas->data);
}

float Font::GetTextWidth(const std::string& text, float scale)
{
    float width = 0;
    for (int i = 0; i < text.size(); i++)
    {
        const char* codepoint = (char*)text.c_str() + i;

        auto glyph = ftgl::texture_font_find_glyph(m_font, codepoint);

        width += glyph->advance_x * scale;
    }

    return width;
}