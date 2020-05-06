#pragma once

#include "Core/Scene.h"
#include "Core/Common.h"

#include "Renderer/Texture.h"
#include "Renderer/Text/Font.h"

#include "Game/Components.h"
#include "Game/Systems.h"

#include "ECS/ECS.h"

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

    Entity m_player;
    Ref<RenderSystem> m_renderSystem;
    Ref<InputSystem> m_inputSystem;

    std::vector<Entity> m_grounds;

    float m_delta;
};