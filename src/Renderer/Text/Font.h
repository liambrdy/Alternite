#pragma once

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Renderer/Text/texture-font.h"
#include "Renderer/Text/texture-atlas.h"

#include "Core/Common.h"

#define FONT_NORMAL BIT(0)
#define FONT_SIGNED_DISTANCE BIT(1)

class Font
{
public:
    Font(const std::string& filepath, int flags);
    virtual ~Font();

    ftgl::texture_glyph_t* GetCharacter(const char* codepoint);

    void BindTexture(int slot = 0) const;

    void RecreateTexture();

    float GetTextWidth(const std::string& text, float scale);

    ftgl::texture_atlas_t* GetAtlas() const { return m_atlas; }

    int GetFlags() const { return  m_flags; }

    bool operator==(const Font& other) { return m_texture == other.m_texture; }
private:
    ftgl::texture_font_t* m_font;
    ftgl::texture_atlas_t* m_atlas;

    uint32_t m_texture;

    int m_flags;
};