#pragma once

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <texture-font.h>
#include <texture-atlas.h>

#include "Core/Common.h"

class Font
{
public:
    Font(const std::string& filepath);
    virtual ~Font();

    ftgl::texture_glyph_t* GetCharacter(const char* codepoint);

    void BindTexture() const;

    void RecreateTexture();

    ftgl::texture_atlas_t* GetAtlas() const { return m_atlas; }
private:
    ftgl::texture_font_t* m_font;
    ftgl::texture_atlas_t* m_atlas;

    uint32_t m_texture;
};