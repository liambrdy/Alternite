#include "Renderer/Font.h"

#include "Core/Common.h"

#include <glad/glad.h>

Font::Font(const std::string& filepath)
{
    if (FT_Init_FreeType(&m_ft))
        ASSERT(false, "Failed to initialize Freetype");

    if (FT_New_Face(m_ft, filepath.c_str(), 0, &m_face))
        ASSERT(false, "Failed to load font");

    FT_Set_Pixel_Sizes(m_face, 0, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (GLubyte c = 32; c < 127; c++)
    {
        if (FT_Load_Char(m_face, c, FT_LOAD_RENDER))
        {
            LOG_ERROR("Failed to load character: {0}", (char)c);
            continue;
            //ASSERT(false, "Failed to load character");
        }

        GLuint texture;
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(m_face->glyph->bitmap.width, m_face->glyph->bitmap.rows),
            glm::ivec2(m_face->glyph->bitmap_left, m_face->glyph->bitmap_top),
            m_face->glyph->advance.x
        };

        m_characters.insert(std::pair<char, Character>(c, character));
    }
}

Font::~Font()
{
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_ft);
}
