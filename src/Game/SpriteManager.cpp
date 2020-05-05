#include "Game/SpriteManager.h"

std::map<std::string, Ref<Texture>> SpriteManager::s_sheets;
std::map<std::string, Sprite> SpriteManager::s_sprites;

void SpriteManager::Init()
{
}

void SpriteManager::Shutdown()
{
    s_sprites.clear();
    s_sheets.clear();
}

void SpriteManager::LoadTexture(const std::string& name, const std::string& filepath)
{
    if (s_sheets.find(name) != s_sheets.end())
        return;

    Ref<Texture> sheet = std::make_shared<Texture>(filepath);
    s_sheets.insert(std::pair(name, sheet));
}

Sprite SpriteManager::LoadSprite(const std::string& name, const std::string& sheetName, const glm::vec2& pos, const glm::vec2& size)
{
    ASSERT(s_sheets.find(sheetName) != s_sheets.end(), "Failed to find sprite sheet for sprite creation");

    auto sheet = s_sheets.at(sheetName);

    glm::vec2 uv1 = { pos.x / sheet->GetWidth(), pos.y / sheet->GetHeight() };
    glm::vec2 uv2 = { (pos.x + size.x) / sheet->GetWidth(), (pos.y + size.y) / sheet->GetHeight() };

    Sprite sprite = {};
    sprite.sheet = sheet;
    sprite.uv1 = uv1;
    sprite.uv2 = uv2;

    s_sprites.insert(std::pair(name, sprite));

    return sprite;
}