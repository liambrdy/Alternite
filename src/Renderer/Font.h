#pragma once

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>

struct Character
{
    uint32_t texID;
    glm::ivec2 size;
    glm::ivec2 bearing;
    FT_Pos advance;
};

class Font
{
public:
    Font(const std::string& filepath);
    virtual ~Font();

    Character GetCharacter(char c) { return m_characters[c]; }
private:
    FT_Library m_ft;
    FT_Face m_face;

    std::map<char, Character> m_characters;
};