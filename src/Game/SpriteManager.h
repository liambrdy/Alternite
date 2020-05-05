#pragma once

#include "Core/Common.h"
#include "Renderer/Texture.h"

#include <glm/glm.hpp>

#include <map>
#include <string>

struct Sprite
{
    Ref<Texture> sheet;
    glm::vec2 uv1, uv2;
};

class SpriteManager
{
public:
    static void Init();
    static void Shutdown();

    static void LoadTexture(const std::string& name, const std::string& filepath);
    static Sprite LoadSprite(const std::string& name, const std::string& sheetName, const glm::vec2& pos, const glm::vec2& size);

    static Sprite GetSprite(const std::string& name) { ASSERT(s_sprites.find(name) != s_sprites.end(), "Failed to find sprite"); return s_sprites.at(name); }
private:
    static std::map<std::string, Ref<Texture>> s_sheets;
    static std::map<std::string, Sprite> s_sprites;
};