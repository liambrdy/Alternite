#pragma once

#include "Core/Scene.h"
#include "Core/Common.h"

#include "Renderer/Texture.h"
#include "Renderer/Text/Font.h"

#include "Game/Entity.h"

#include <glm/glm.hpp>

#include <vector>

class Game : public Scene
{
public:
    Game();
    virtual ~Game();

    virtual Scene* OnUpdate(double delta) override;
    virtual void OnRender() const override;
private:
    Ref<Font> m_font;

    std::vector<Entity*> m_entities;
    Entity* m_player;

    float m_delta;
};