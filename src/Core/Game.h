#pragma once

#include "Core/Scene.h"
#include "Core/Common.h"

#include "Renderer/Texture.h"
#include "Renderer/Text/Font.h"

#include <glm/glm.hpp>

class Game : public Scene
{
public:
    Game();
    virtual ~Game();

    virtual Scene* OnUpdate(double delta) override;
    virtual void OnRender() const override;
private:
    Ref<Texture> m_texture;
    Ref<Font> m_font;

    Ref<Texture> m_ground, m_player;
    glm::vec2 m_pos = { 600, 400 };

    float m_delta;
};